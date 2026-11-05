#ifndef AUTOAPP_WIRELESSSERVICE_H
#define AUTOAPP_WIRELESSSERVICE_H

#include <QBluetoothAddress>
#include <QBluetoothDeviceDiscoveryAgent>
#include <QBluetoothDeviceInfo>
#include <QBluetoothServer>
#include <QBluetoothServiceInfo>
#include <QList>
#include <QObject>

#include "../logging/loggingCategory.h"

class BluetoothDevice;
class NetworkService;

// ponytail: manager half of Android Auto Wireless - the analog of UsbService. Powers the BT
// adapter, runs discovery, registers the SDP service head units advertise and listens on
// RFCOMM. A dial is routed to the matching phone; the phone (BluetoothDevice) owns its session.
// The TCP projection listener lives in AndroidAutoService::startWirelessServer().
class WirelessService : public QObject {
    Q_OBJECT

public:
    Q_PROPERTY(QList<QObject *> devices MEMBER devices NOTIFY devicesChanged)

    explicit WirelessService(NetworkService &new_networkService, QObject *parent = nullptr);
    ~WirelessService() override;

    bool start();

signals:
    void handshakeCompleted();
    void devicesChanged();

public slots:
    void startDiscovery();

private slots:
    void onClientConnected();
    void onDeviceDiscovered(const QBluetoothDeviceInfo &info);

private:
    QLoggingCategory category;

    NetworkService &networkService;
    QBluetoothServer rfcommServer;
    QBluetoothServiceInfo serviceInfo;
    QBluetoothDeviceDiscoveryAgent *discoveryAgent = nullptr;
    QList<QObject *> devices;

    void addKnownDevice(const QString &name, const QBluetoothAddress &address);
    BluetoothDevice *findDevice(const QBluetoothAddress &address);
};

#endif  // AUTOAPP_WIRELESSSERVICE_H
