#include "wirelessAndroidAutoDevice.h"

#include "androidAutoService.h"
#include "f1x/aasdk/TCP/TCPEndpoint.hpp"

WirelessAndroidAutoDevice::WirelessAndroidAutoDevice(
    QObject *parent,
    f1x::aasdk::tcp::ITCPEndpoint::SocketPointer new_socket,
    boost::asio::io_context &new_ioService,
    f1x::openauto::autoapp::service::AndroidAutoEntityFactory &new_androidAutoEntityFactory)
    : AndroidAutoDevice(parent, "WIRELESS ANDROID AUTO DEVICE", new_ioService, new_androidAutoEntityFactory),
      socket(new_socket) {
    cinfo << "Wireless AA client connected (server mode)";
}

WirelessAndroidAutoDevice::~WirelessAndroidAutoDevice() {
    stop();
    close();
}

void WirelessAndroidAutoDevice::open() {}

void WirelessAndroidAutoDevice::close() {}

void WirelessAndroidAutoDevice::start() {
    // Runs on the Qt (GUI) thread - the same thread the USB path creates the entity on.
    // The factory builds the video/audio QObjects here, so they must get GUI-thread affinity or
    // their gstreamer timers cannot run (empty video). androidAutoEntity->start() re-dispatches to
    // its own io strand, so nothing blocks the GUI thread.
    if (androidAutoEntity != nullptr) {
        cinfo << "AA entity already running, skipping start.";
        return;
    }

    cinfo << "Starting AA entity";

    try {
        auto tcpEndpoint = std::make_shared<f1x::aasdk::tcp::TCPEndpoint>(tcpWrapper, std::move(socket));

        androidAutoEntity = androidAutoEntityFactory.create(std::move(tcpEndpoint));
        androidAutoEntity->start(*((AndroidAutoService *)(parent())));

        cinfo << "Started AA entity";
    } catch (const std::exception &e) {
        cerror << "Error while starting AA entity: " << e.what();
        androidAutoEntity.reset();
    }
}

void WirelessAndroidAutoDevice::stop() {
    if (androidAutoEntity != nullptr) {
        androidAutoEntity->stop();
        androidAutoEntity.reset();
    }
}
