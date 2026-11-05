#ifndef AUTOAPP_BLUETOOTHDEVICE_H
#define AUTOAPP_BLUETOOTHDEVICE_H

#include <QBluetoothAddress>
#include <QByteArray>
#include <QObject>

#include "../logging/loggingCategory.h"

class QBluetoothSocket;
class NetworkService;

// ponytail: a phone in the wireless list - the analog of UsbDevice, and the single owner of
// that phone's state. Tapping it arms the phone for Android Auto; the phone then dials our
// RFCOMM/SDP service and this object runs the WPP handshake that gets it onto the Wi-Fi AP
// (owned elsewhere - NetworkService) and, on success, onto projection.
class BluetoothDevice : public QObject {
    Q_OBJECT

public:
    Q_PROPERTY(QString displayName MEMBER name CONSTANT)
    Q_PROPERTY(QString address MEMBER addressString CONSTANT)
    Q_PROPERTY(bool aaRequested MEMBER aaRequested NOTIFY aaRequestedChanged)

    BluetoothDevice(QString new_name, const QBluetoothAddress &new_address, NetworkService &new_networkService);
    ~BluetoothDevice() override;

    bool isAaRequested() const { return aaRequested; }
    bool hasSession() const { return socket != nullptr; }

    // Called by WirelessService when this phone dials RFCOMM: adopts the socket and runs the
    // WPP exchange that gets the phone onto the shared Wi-Fi AP.
    void startSession(QBluetoothSocket *socket);

signals:
    void aaRequestedChanged();
    void handshakeCompleted();
    void sessionEnded();

public slots:
    void activate();

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
    QBluetoothAddress btAddress;
    NetworkService &networkService;
    bool aaRequested = false;
    QBluetoothSocket *socket = nullptr;
    QByteArray rxBuffer;
};

#endif  // AUTOAPP_BLUETOOTHDEVICE_H
