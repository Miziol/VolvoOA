#ifndef AUTOAPP_WIRELESSANDROIDAUTODEVICE_H
#define AUTOAPP_WIRELESSANDROIDAUTODEVICE_H

#include <libusb.h>

#include "androidAutoDevice.h"
#include "f1x/aasdk/TCP/TCPWrapper.hpp"
#include "f1x/openauto/autoapp/Service/AndroidAutoEntity.hpp"
#include "f1x/openauto/autoapp/Service/AndroidAutoEntityFactory.hpp"

class WirelessAndroidAutoDevice : public AndroidAutoDevice {
    Q_OBJECT

signals:

public:
    WirelessAndroidAutoDevice(QObject *parent,
                              QString new_ipAddress,
                              boost::asio::io_service &new_ioService,
                              f1x::openauto::autoapp::service::AndroidAutoEntityFactory &new_androidAutoEntityFactory);
    ~WirelessAndroidAutoDevice();

private:
    f1x::aasdk::tcp::ITCPEndpoint::SocketPointer socket;
    f1x::aasdk::tcp::TCPWrapper tcpWrapper;
    QString ipAddress;

public slots:
    void open() override;
    void close() override;
    void start() override;
    void stop() override;

private:
    void connectHandler(const boost::system::error_code &ec);
};

#endif  // AUTOAPP_WIRELESSANDROIDAUTODEVICE_H
