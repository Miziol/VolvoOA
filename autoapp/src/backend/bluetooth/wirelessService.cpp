#include "wirelessService.h"

#include <QBluetoothLocalDevice>
#include <QBluetoothUuid>

#include "bluetoothDevice.h"

namespace {
// RFCOMM service UUID + name real Android Auto Wireless head units register.
const QLatin1String AA_WIRELESS_SERVICE_UUID("4de17a00-52cb-11e6-bdf4-0800200c9a66");
const QLatin1String AA_WIRELESS_SERVICE_NAME("AndroidAutoWirelessHU");
}  // namespace

WirelessService::WirelessService(NetworkService &new_networkService, QObject *parent)
    : QObject(parent),
      category("WIRELESS SERVICE"),
      networkService(new_networkService),
      rfcommServer(QBluetoothServiceInfo::RfcommProtocol, this) {
    connect(&rfcommServer, &QBluetoothServer::newConnection, this, &WirelessService::onClientConnected);
}

WirelessService::~WirelessService() {
    serviceInfo.unregisterService();
    rfcommServer.close();
}

bool WirelessService::start() {
    QBluetoothLocalDevice localDevice;
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

    // Seed with already-paired phones: they need not be discoverable.
    for (const QBluetoothAddress &paired : localDevice.connectedDevices()) {
        addKnownDevice(paired.toString(), paired);
    }

    discoveryAgent = new QBluetoothDeviceDiscoveryAgent(this);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered, this,
            &WirelessService::onDeviceDiscovered);
    connect(discoveryAgent, &QBluetoothDeviceDiscoveryAgent::errorOccurred, this,
            [this](QBluetoothDeviceDiscoveryAgent::Error error) { cwarning << "BT discovery error:" << int(error); });
    startDiscovery();

    return true;
}

void WirelessService::startDiscovery() {
    if (discoveryAgent == nullptr) {
        return;
    }
    cinfo << "Starting BT discovery";
    discoveryAgent->start();
}

void WirelessService::addKnownDevice(const QString &name, const QBluetoothAddress &address) {
    if (findDevice(address) != nullptr) {
        return;
    }
    if (address.isNull()) {
        return;
    }

    BluetoothDevice *phone = new BluetoothDevice(name, address, networkService);
    connect(phone, &BluetoothDevice::handshakeCompleted, this, &WirelessService::handshakeCompleted);
    devices.append(phone);
    emit devicesChanged();
}

BluetoothDevice *WirelessService::findDevice(const QBluetoothAddress &address) {
    const QString wanted = address.toString();
    for (auto *obj : devices) {
        if (obj->property("address").toString() == wanted) {
            return qobject_cast<BluetoothDevice *>(obj);
        }
    }
    return nullptr;
}

void WirelessService::onDeviceDiscovered(const QBluetoothDeviceInfo &info) {
    if (info.majorDeviceClass() != QBluetoothDeviceInfo::PhoneDevice) {
        return;
    }

    // Main view lists only paired & available phones; a merely-discovered, unpaired phone is noise.
    QBluetoothLocalDevice localDevice;
    if (localDevice.pairingStatus(info.address()) != QBluetoothLocalDevice::Paired) {
        return;
    }

    const QString name = info.name().isEmpty() ? info.address().toString() : info.name();
    cinfo << "Paired phone available:" << name << info.address().toString();
    addKnownDevice(name, info.address());
}

void WirelessService::onClientConnected() {
    QBluetoothSocket *socket = rfcommServer.nextPendingConnection();
    if (socket == nullptr) {
        cerror << "Received null socket on new connection";
        return;
    }

    // Only serve a phone the user armed; otherwise drop the link at once (the phone retries
    // once armed) so the single-radio hotspot stays off until a phone actually asks for AA.
    BluetoothDevice *known = findDevice(socket->peerAddress());
    if (known == nullptr || !known->isAaRequested()) {
        cinfo << "Ignoring RFCOMM request from" << socket->peerAddress().toString() << "(not requested)";
        socket->deleteLater();
        return;
    }

    known->startSession(socket);
}
