#ifndef AUTOAPP_BLUETOOTHSERVICE_H
#define AUTOAPP_BLUETOOTHSERVICE_H

#include <QBluetoothAddress>
#include <QBluetoothLocalDevice>
#include <QBluetoothServer>
#include <QBluetoothServiceInfo>
#include <QList>
#include <QObject>

#include "../logging/loggingCategory.h"

class BluetoothDevice;
class NetworkService;

// ponytail: the Bluetooth half of Android Auto Wireless - the analog of UsbService. Powers the
// BT adapter, advertises it (discoverable) and registers the SDP service + RFCOMM listener head
// units expose, so a phone pairs and connects to us (classic phone -> head unit). A dial is
// routed to the matching phone; the phone (BluetoothDevice) owns its session.
// The TCP projection listener lives in AndroidAutoService::startWirelessServer().
class BluetoothService : public QObject {
    Q_OBJECT

public:
    Q_PROPERTY(QList<QObject *> bluetoothDevices MEMBER devices NOTIFY bluetoothDevicesChanged)
    Q_PROPERTY(bool discoverable READ isDiscoverable WRITE setDiscoverable NOTIFY discoverableChanged)

    explicit BluetoothService(NetworkService &new_networkService, QObject *parent = nullptr);
    ~BluetoothService() override;

    bool start();
    bool isDiscoverable() const { return discoverable; }

signals:
    void handshakeCompleted();
    void bluetoothDevicesChanged();
    void discoverableChanged();

public slots:
    void setDiscoverable(bool enabled);

private slots:
    void onClientConnected();
    void refreshDevices();

private:
    QLoggingCategory category;

    NetworkService &networkService;
    QBluetoothLocalDevice localDevice;
    QBluetoothServer rfcommServer;
    QBluetoothServiceInfo serviceInfo;
    bool discoverable = false;
    QList<QObject *> devices;

    void addKnownDevice(const QString &name, const QBluetoothAddress &address);
    BluetoothDevice *findDevice(const QBluetoothAddress &address);
};

#endif  // AUTOAPP_BLUETOOTHSERVICE_H
