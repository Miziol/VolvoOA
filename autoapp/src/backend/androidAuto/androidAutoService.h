#ifndef AUTOAPP_ANDROIDAUTOSERVICE_H
#define AUTOAPP_ANDROIDAUTOSERVICE_H

#include <libusb-1.0/libusb.h>

#include <QObject>
#include <QTimer>
#include <boost/asio/ip/tcp.hpp>
#include <memory>

#include "../logging/loggingCategory.h"
#include "../settings/settingsManager.h"
#include "f1x/openauto/autoapp/Service/AndroidAutoEntityFactory.hpp"
#include "f1x/openauto/autoapp/Service/ServiceFactory.hpp"
#include "usbAndroidAutoDevice.h"
#include "wirelessAndroidAutoDevice.h"

class AndroidAutoService : public QObject, public f1x::openauto::autoapp::service::IAndroidAutoEntityEventHandler {
    Q_OBJECT

signals:
    void aaDeviceChanged();
    void focusOnAA(bool focus);
    void connectionStateChanged();

public:
    Q_PROPERTY(bool aaConnected READ isAADeviceConnected NOTIFY aaDeviceChanged);
    Q_PROPERTY(bool aaBusy READ isAABusy NOTIFY connectionStateChanged);

public:
    AndroidAutoService(SettingsManager &new_settings, boost::asio::io_context &new_ioService);
    ~AndroidAutoService();

public slots:
    void addUSBDevice(libusb_context *context, libusb_device *device);
    void removeDevice(libusb_device *device);
    void startWirelessServer();
    void armWirelessProjection();

    void startIOServiceWorkers(boost::asio::io_context &ioService, std::vector<std::thread> &threadPool);
    void createFactories(QObject *qmlRootObject);

    bool isAADeviceConnected();

    // Transport-agnostic connection control: a list entry (USB or wireless) hands over its
    // source object and the service owns the single AA slot from that click until disconnect.
    bool connectDevice(QObject *device);
    void disconnectDevice();

    void onAndroidAutoQuit() override;
    void setFocusOnAA(bool focus) override;

private:
    // The transport (USB hotplug / wireless TCP dial) stays hidden behind aaDevice; the service
    // only tracks whether its single slot is free, claimed by a click, or already in use.
    enum class ConnectionState { Idle, Connecting, Connected };

    void setConnectionState(ConnectionState newState);
    bool isAABusy() const;

    // Wireless twin of the addUSBDevice slot above: same transport entry point, but
    // fed by the TCP accept loop instead of USB hotplug.
    void addWirelessDevice(f1x::aasdk::tcp::ITCPEndpoint::SocketPointer socket);
    // Both transports (USB hotplug, wireless TCP dial) funnel through here so the
    // "one AA entity at a time" rule and the open()->start() sequence live in one place.
    void installAndroidAutoDevice(AndroidAutoDevice *device);
    // Re-arms the TCP accept loop; also called once at startup by startWirelessServer().
    void acceptNextWirelessClient();

    QLoggingCategory category;
    SettingsManager &settingsManager;

    boost::asio::io_context &ioService;
    f1x::openauto::autoapp::service::ServiceFactory *serviceFactory;
    f1x::openauto::autoapp::service::AndroidAutoEntityFactory *androidAutoEntityFactory;

    AndroidAutoDevice *aaDevice;
    ConnectionState state = ConnectionState::Idle;
    // ponytail: a claim that never resolves (pairing refused, phone never dials) would wedge the
    // slot; one timer frees it. Tune to the slowest real handshake, not the fastest.
    QTimer connectionTimeout;

    // The phone dials this port after joining our AP.
    static constexpr uint16_t AA_PORT = 5277;
    std::unique_ptr<boost::asio::ip::tcp::acceptor> wirelessAcceptor;
    // Armed only after the phone completed the WPP/BT handshake, so a stale or
    // premature TCP dial cannot spawn a doomed AA entity.
    bool wirelessReady = false;
};

#endif  // AUTOAPP_ANDROIDAUTOSERVICE_H
