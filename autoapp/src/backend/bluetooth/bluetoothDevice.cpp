#include "bluetoothDevice.h"

#include <QBluetoothSocket>

#include "../network/networkService.h"

namespace {
// WPP message ids (aa-proxy-rs "ProxyMessageId").
constexpr quint16 MSG_WIFI_START_REQUEST = 1;
constexpr quint16 MSG_WIFI_INFO_REQUEST = 2;
constexpr quint16 MSG_WIFI_INFO_RESPONSE = 3;
constexpr quint16 MSG_WIFI_CONNECT_STATUS = 6;
constexpr quint16 MSG_WIFI_START_RESPONSE = 7;
constexpr quint16 MSG_PING_REQUEST = 8;
constexpr quint16 MSG_PING_RESPONSE = 9;

// WifiInfoResponse enum values.
constexpr quint64 SECURITY_WPA2_PERSONAL = 8;
constexpr quint64 AP_TYPE_STATIC = 0;

// ponytail: matches AndroidAutoService::AA_PORT; the phone dials this after joining.
constexpr quint16 AA_PROJECTION_PORT = 5277;

QByteArray encodeVarint(quint64 value) {
    QByteArray out;
    while (true) {
        quint8 byte = value & 0x7F;
        value >>= 7;
        if (value != 0) {
            out.append(char(byte | 0x80));
        } else {
            out.append(char(byte));
            break;
        }
    }
    return out;
}

QByteArray encodeTag(quint32 field, quint8 wireType) {
    return encodeVarint((field << 3) | wireType);
}

QByteArray encodeStringField(quint32 field, const QString &value) {
    const QByteArray utf8 = value.toUtf8();
    return encodeTag(field, 2) + encodeVarint(utf8.size()) + utf8;
}

QByteArray encodeVarintField(quint32 field, quint64 value) {
    return encodeTag(field, 0) + encodeVarint(value);
}

bool readVarint(const QByteArray &data, int &offset, quint64 &out) {
    out = 0;
    int shift = 0;
    while (offset < data.size()) {
        const quint8 byte = quint8(data[offset++]);
        out |= quint64(byte & 0x7F) << shift;
        if ((byte & 0x80) == 0) return true;
        shift += 7;
        if (shift > 63) return false;
    }
    return false;
}

// Returns the value of the first varint-typed field matching `field`, or -1 if absent.
qint64 readVarintField(const QByteArray &payload, quint32 field) {
    int offset = 0;
    while (offset < payload.size()) {
        quint64 key = 0;
        if (!readVarint(payload, offset, key)) return -1;
        const quint32 fieldNumber = key >> 3;
        const quint8 wireType = key & 0x7;
        if (wireType == 0) {
            quint64 value = 0;
            if (!readVarint(payload, offset, value)) return -1;
            if (fieldNumber == field) return qint64(value);
        } else if (wireType == 2) {
            quint64 length = 0;
            if (!readVarint(payload, offset, length)) return -1;
            offset += int(length);
        } else if (wireType == 5) {
            offset += 4;
        } else if (wireType == 1) {
            offset += 8;
        } else {
            return -1;
        }
    }
    return -1;
}
}  // namespace

BluetoothDevice::BluetoothDevice(QString new_name, const QBluetoothAddress &new_address, NetworkService &new_networkService)
    : category("BT DEVICE"),
      name(new_name),
      addressString(new_address.toString()),
      networkService(new_networkService) {}

BluetoothDevice::~BluetoothDevice() {
    // The network service outlives this phone entry, so drop our connections to it.
    disconnect(&networkService, nullptr, this, nullptr);
    if (socket != nullptr) {
        socket->deleteLater();
    }
}

QString BluetoothDevice::displayName() {
    return name;
}

void BluetoothDevice::activate() {
    // The phone initiates the BT pairing and the RFCOMM dial (classic phone -> head unit), so we
    // only record consent here: until tapped, onClientConnected() drops this phone's dials.
    if (!armed) {
        armed = true;
        cinfo << name << "armed for Android Auto";
    }
}

void BluetoothDevice::startSession(QBluetoothSocket *newSocket) {
    if (socket != nullptr) {
        cinfo << "Session already active for" << name << "- ignoring extra dial";
        newSocket->deleteLater();
        return;
    }

    socket = newSocket;
    connect(socket, &QBluetoothSocket::readyRead, this, &BluetoothDevice::onReadyRead);
    connect(socket, &QBluetoothSocket::disconnected, this, &BluetoothDevice::onSocketDisconnected);
    connect(&networkService, &NetworkService::ready, this, &BluetoothDevice::onAccessPointReady);
    connect(&networkService, &NetworkService::failed, this, &BluetoothDevice::onAccessPointFailed);

    cinfo << "Phone connected:" << name << socket->peerAddress().toString();

    // The phone cannot join until our AP exists, so bring it up first and only then send
    // WifiStartRequest (matches the working ngocthang ordering).
    if (networkService.isAccessPointActive()) {
        sendStartRequest();
        return;
    }
    networkService.ensureStarted();
}

void BluetoothDevice::onSocketDisconnected() {
    cinfo << "Phone disconnected" << name;
    disconnect(&networkService, nullptr, this, nullptr);
    socket->deleteLater();
    socket = nullptr;
    rxBuffer.clear();
    if (connected) {
        connected = false;
        emit connectedChanged();
    }
    emit sessionEnded();
}

void BluetoothDevice::onAccessPointReady() {
    cinfo << "Access point ready:" << networkService.info().ssid << "gateway" << networkService.info().address;
    if (socket != nullptr) {
        sendStartRequest();
    }
}

void BluetoothDevice::onAccessPointFailed(const QString &reason) {
    cerror << "Access point failed:" << reason << "- phone cannot join the network";
}

void BluetoothDevice::onReadyRead() {
    rxBuffer += socket->readAll();

    while (rxBuffer.size() >= 4) {
        const quint16 length = (quint8(rxBuffer[0]) << 8) | quint8(rxBuffer[1]);
        const quint16 messageId = (quint8(rxBuffer[2]) << 8) | quint8(rxBuffer[3]);

        if (rxBuffer.size() < 4 + length) {
            break;
        }

        const QByteArray payload = rxBuffer.mid(4, length);
        rxBuffer.remove(0, 4 + length);

        handleMessage(messageId, payload);
    }
}

void BluetoothDevice::sendFrame(quint16 messageId, const QByteArray &payload) {
    if (socket == nullptr) {
        return;
    }

    QByteArray frame;
    frame.append(char((payload.size() >> 8) & 0xFF));
    frame.append(char(payload.size() & 0xFF));
    frame.append(char((messageId >> 8) & 0xFF));
    frame.append(char(messageId & 0xFF));
    frame.append(payload);

    socket->write(frame);
}

void BluetoothDevice::sendStartRequest() {
    // The phone dials this address once it is on our network, so it must be the real gateway
    // (= us). "0.0.0.0" is only the phone's documented "dial the gateway" form for wpp_info
    // on type 4, not for WifiStartRequest: a real phone joins (status 0) against "0.0.0.0"
    // but never dials it.
    QString address = networkService.info().address;
    if (address.isEmpty()) {
        address = "0.0.0.0";
    }
    const QByteArray payload = encodeStringField(1, address) + encodeVarintField(2, AA_PROJECTION_PORT);
    sendFrame(MSG_WIFI_START_REQUEST, payload);
    cinfo << "Stage 1/5: sent WifiStartRequest(ip" << address << "port" << AA_PROJECTION_PORT << ")";
}

void BluetoothDevice::sendInfoResponse() {
    const WifiApInfo ap = networkService.info();
    const QByteArray payload = encodeStringField(1, ap.ssid) +
                               encodeStringField(2, ap.key) +
                               encodeStringField(3, ap.bssid) +
                               encodeVarintField(4, SECURITY_WPA2_PERSONAL) + encodeVarintField(5, AP_TYPE_STATIC);
    sendFrame(MSG_WIFI_INFO_RESPONSE, payload);
    cinfo << "Stage 3/5: sent WifiInfoResponse(ssid" << ap.ssid << ")";
}

void BluetoothDevice::handleMessage(quint16 messageId, const QByteArray &payload) {
    switch (messageId) {
        case MSG_WIFI_INFO_REQUEST:
            cinfo << "Stage 2/5: WifiInfoRequest received";
            sendInfoResponse();
            break;
        case MSG_WIFI_START_RESPONSE:
            cinfo << "Stage 4/5: WifiStartResponse received";
            break;
        case MSG_WIFI_CONNECT_STATUS: {
            const qint64 status = readVarintField(payload, 1);
            cinfo << "Stage 5/5: WifiConnectStatus received, status" << status;
            if (status == 0) {
                if (!connected) {
                    connected = true;
                    emit connectedChanged();
                }
                emit handshakeCompleted();
            }
            break;
        }
        case MSG_PING_REQUEST:
            sendFrame(MSG_PING_RESPONSE, QByteArray());
            break;
        default:
            cinfo << "Unhandled WPP message id" << messageId;
            break;
    }
}
