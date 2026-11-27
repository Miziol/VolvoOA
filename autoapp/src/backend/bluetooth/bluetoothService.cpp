#include "bluetoothService.h"

#include <QBluetoothLocalDevice>
#include <QBluetoothUuid>

#include "bluetoothDevice.h"

namespace {
// RFCOMM service UUID + name real Android Auto Wireless head units register.
const QLatin1String AA_WIRELESS_SERVICE_UUID("4de17a00-52cb-11e6-bdf4-0800200c9a66");
const QLatin1String AA_WIRELESS_SERVICE_NAME("AndroidAutoWirelessHU");
}  // namespace

BluetoothService::BluetoothService(NetworkService &new_networkService, QObject *parent)
    : QObject(parent),
      category("BLUETOOTH SERVICE"),
      networkService(new_networkService),
      rfcommServer(QBluetoothServiceInfo::RfcommProtocol, this) {
    connect(&rfcommServer, &QBluetoothServer::newConnection, this, &BluetoothService::onClientConnected);
}

BluetoothService::~BluetoothService() {
    serviceInfo.unregisterService();
    rfcommServer.close();
}

bool BluetoothService::start() {
    if (!localDevice.isValid()) {
        cerror << "No Bluetooth adapter available, wireless handshake disabled";
        return false;
    }

    const QBluetoothAddress localAddress = localDevice.address();
    if (localDevice.hostMode() == QBluetoothLocalDevice::HostPoweredOff) {
        localDevice.powerOn();
    }
    localDevice.setHostMode(QBluetoothLocalDevice::HostConnectable);

    if (!rfcommServer.listen(localAddress)) {
        cerror << "Failed to listen on RFCOMM, error:" << int(rfcommServer.error());
        return false;
    }

    const quint16 port = rfcommServer.serverPort();

    QBluetoothServiceInfo::Sequence classId;
    classId << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::SerialPort));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BluetoothProfileDescriptorList, classId);
    classId.prepend(QVariant::fromValue(QBluetoothUuid(AA_WIRELESS_SERVICE_UUID)));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceClassIds, classId);
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceName, AA_WIRELESS_SERVICE_NAME);
    serviceInfo.setAttribute(QBluetoothServiceInfo::ServiceDescription, "AndroidAuto wireless projection setup");

    QBluetoothServiceInfo::Sequence publicBrowse;
    publicBrowse << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::ServiceClassUuid::PublicBrowseGroup));
    serviceInfo.setAttribute(QBluetoothServiceInfo::BrowseGroupList, publicBrowse);

    QBluetoothServiceInfo::Sequence protocolDescriptorList;
    QBluetoothServiceInfo::Sequence protocol;
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::ProtocolUuid::L2cap));
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    protocol.clear();
    protocol << QVariant::fromValue(QBluetoothUuid(QBluetoothUuid::ProtocolUuid::Rfcomm)) << QVariant::fromValue(port);
    protocolDescriptorList.append(QVariant::fromValue(protocol));
    serviceInfo.setAttribute(QBluetoothServiceInfo::ProtocolDescriptorList, protocolDescriptorList);
    serviceInfo.setServiceUuid(QBluetoothUuid(AA_WIRELESS_SERVICE_UUID));

    if (!serviceInfo.registerService(localAddress)) {
        cerror << "Failed to register SDP service";
        return false;
    }

    cinfo << "RFCOMM listening on" << localAddress.toString() << "port" << port;

    // The phone pairs itself (us -> discoverable) and connects to us; we only list the phones it
    // has already paired. Re-read the list whenever a pairing completes so it stays current.
    connect(&localDevice, &QBluetoothLocalDevice::pairingFinished, this, &BluetoothService::refreshDevices);
    refreshDevices();

    return true;
}

void BluetoothService::setDiscoverable(bool enabled) {
    if (discoverable == enabled) {
        return;
    }
    discoverable = enabled;
    localDevice.setHostMode(enabled ? QBluetoothLocalDevice::HostDiscoverable : QBluetoothLocalDevice::HostConnectable);
    cinfo << "Bluetooth" << (enabled ? "discoverable" : "connectable only");
    emit discoverableChanged();
}

void BluetoothService::refreshDevices() {
    for (const QBluetoothAddress &paired : localDevice.connectedDevices()) {
        addKnownDevice(paired.toString(), paired);
    }
}

void BluetoothService::addKnownDevice(const QString &name, const QBluetoothAddress &address) {
    if (findDevice(address) != nullptr) {
        return;
    }
    if (address.isNull()) {
        return;
    }

    BluetoothDevice *phone = new BluetoothDevice(name, address, networkService);
    connect(phone, &BluetoothDevice::handshakeCompleted, this, &BluetoothService::handshakeCompleted);
    devices.append(phone);
    emit bluetoothDevicesChanged();
}

BluetoothDevice *BluetoothService::findDevice(const QBluetoothAddress &address) {
    const QString wanted = address.toString();
    for (auto *obj : devices) {
        if (obj->property("address").toString() == wanted) {
            return qobject_cast<BluetoothDevice *>(obj);
        }
    }
    return nullptr;
}

void BluetoothService::onClientConnected() {
    QBluetoothSocket *socket = rfcommServer.nextPendingConnection();
    if (socket == nullptr) {
        cerror << "Received null socket on new connection";
        return;
    }

    // Only serve a phone the user armed; otherwise drop the link at once (the phone retries
    // once armed) so the single-radio hotspot stays off until a phone actually asks for AA.
    BluetoothDevice *known = findDevice(socket->peerAddress());
    if (known == nullptr || !known->isArmed()) {
        cinfo << "Ignoring RFCOMM request from" << socket->peerAddress().toString() << "(not requested)";
        socket->deleteLater();
        return;
    }

    known->startSession(socket);
}
