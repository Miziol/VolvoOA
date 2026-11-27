#ifndef AUTOAPP_WIRELESSANDROIDAUTODEVICE_H
#define AUTOAPP_WIRELESSANDROIDAUTODEVICE_H

#include <libusb.h>

#include <boost/asio/ip/tcp.hpp>
#include <memory>

#include "../logging/loggingCategory.h"
#include "androidAutoDevice.h"
#include "f1x/aasdk/TCP/ITCPEndpoint.hpp"
#include "f1x/aasdk/TCP/TCPWrapper.hpp"
#include "f1x/openauto/autoapp/Service/AndroidAutoEntity.hpp"
#include "f1x/openauto/autoapp/Service/AndroidAutoEntityFactory.hpp"

class WirelessAndroidAutoDevice : public AndroidAutoDevice {
    Q_OBJECT

public:
    WirelessAndroidAutoDevice(QObject *parent,
                              f1x::aasdk::tcp::ITCPEndpoint::SocketPointer newSocket,
                              boost::asio::io_context &new_ioService,
                              f1x::openauto::autoapp::service::AndroidAutoEntityFactory &new_androidAutoEntityFactory);
    ~WirelessAndroidAutoDevice();

private:
    // The accepted projection socket in aasdk's native form (shared_ptr<asio::tcp::socket>): the
    // AA transport is boost::asio and aasdk::tcp::TCPEndpoint consumes exactly this type. Qt owns
    // only the WPP control channel (QBluetoothSocket); the projection channel stays asio.
    f1x::aasdk::tcp::ITCPEndpoint::SocketPointer socket;
    // The wireless transport's wrapper lives with the session it serves; the aasdk TCPEndpoint
    // borrows it by reference, and ~WirelessAndroidAutoDevice tears the entity down first.
    f1x::aasdk::tcp::TCPWrapper tcpWrapper;

public slots:
    void open() override;
    void close() override;
    void start() override;
    void stop() override;
};

#endif  // AUTOAPP_WIRELESSANDROIDAUTODEVICE_H
