#ifndef AUTOAPP_BLUETOOTHDEVICE_H
#define AUTOAPP_BLUETOOTHDEVICE_H

#include <QBluetoothAddress>
#include <QByteArray>

#include "../androidAuto/aaDeviceEntry.h"
#include "../logging/loggingCategory.h"

class QBluetoothSocket;
class NetworkService;

// ponytail: a phone in the wireless list - the analog of UsbDevice, and the single owner of
// that phone's state. Tapping it arms the phone for Android Auto; the phone then dials our
// RFCOMM/SDP service and this object runs the WPP handshake that gets it onto the Wi-Fi AP
// (owned elsewhere - NetworkService) and, on success, onto projection.
class BluetoothDevice : public AaDeviceEntry {
    Q_OBJECT

public:
    Q_PROPERTY(QString address MEMBER addressString CONSTANT)
    // Twin of UsbAndroidAutoDevice's connection state: true while this phone's AA session is up.
    Q_PROPERTY(bool connected MEMBER connected NOTIFY connectedChanged)

    BluetoothDevice(QString new_name, const QBluetoothAddress &new_address, NetworkService &new_networkService);
    ~BluetoothDevice() override;

    QString displayName() const override;
    Type type() const override { return Type::Wireless; }
    // Engaged once the phone has dialed RFCOMM (the socket is held).
    bool isActive() const override { return socket != nullptr; }

    // Consent gate: the phone is only served once the user tapped it in the list (see activate()).
    bool isArmed() const { return armed; }

    // Called by BluetoothService when this phone dials RFCOMM: adopts the socket and runs the
    // WPP exchange that gets the phone onto the shared Wi-Fi AP.
    void startSession(QBluetoothSocket *socket);

signals:
    void connectedChanged();
    void handshakeCompleted();
    void sessionEnded();

public slots:
    // Twin of UsbDevice::activate(): the list-entry start action. Records the user's consent so
    // the phone may dial our RFCOMM/SDP service (the phone itself initiates the connection).
    void activate() override;
    void deactivate() override;

private slots:
    void onReadyRead();
    void onSocketDisconnected();
    void onAccessPointReady();
    void onAccessPointFailed(const QString &reason);

private:
    void sendFrame(quint16 messageId, const QByteArray &payload);
    void handleMessage(quint16 messageId, const QByteArray &payload);
    void sendStartRequest();
    void sendInfoResponse();

    QLoggingCategory category;
    QString name;
    QString addressString;
    NetworkService &networkService;
    bool armed = false;
    bool connected = false;
    QBluetoothSocket *socket = nullptr;
    QByteArray rxBuffer;
};

#endif  // AUTOAPP_BLUETOOTHDEVICE_H
