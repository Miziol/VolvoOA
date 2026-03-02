#include "wirelessAndroidAutoDevice.h"

#include "androidAutoService.h"
#include "f1x/aasdk/TCP/TCPEndpoint.hpp"

WirelessAndroidAutoDevice::WirelessAndroidAutoDevice(
    QObject *parent,
                        QString new_ipAddress,
    boost::asio::io_service &new_ioService,
    f1x::openauto::autoapp::service::AndroidAutoEntityFactory &new_androidAutoEntityFactory)
    : QObject(parent),
      category("WIRELESS ANDROID AUTO DEVICE"),
    socket(nullptr),
      ipAddress(new_ipAddress),
      ioService(new_ioService),
      androidAutoEntityFactory(new_androidAutoEntityFactory),
      androidAutoEntity(nullptr) {
    open();
}

WirelessAndroidAutoDevice::~WirelessAndroidAutoDevice() {
    stop();
    close();
}

void WirelessAndroidAutoDevice::open() {
    socket = std::make_shared<boost::asio::ip::tcp::socket>(ioService);

    tcpWrapper.asyncConnect(*socket, ipAddress.toStdString(), 5277, std::bind(&WirelessAndroidAutoDevice::connectHandler, this, std::placeholders::_1));
}

void WirelessAndroidAutoDevice::close() {
}

void WirelessAndroidAutoDevice::start() {
    if (androidAutoEntity != nullptr)
        return;

    cinfo << "Starting AA entity";

    auto tcpEndpoint(std::make_shared<f1x::aasdk::tcp::TCPEndpoint>(tcpWrapper, std::move(socket)));
    androidAutoEntity = androidAutoEntityFactory.create(std::move(tcpEndpoint));
    androidAutoEntity->start(*((AndroidAutoService *)(parent())));

    cinfo << "Started AA entity";
}

void WirelessAndroidAutoDevice::stop() {
    if (androidAutoEntity != nullptr) {
        androidAutoEntity->stop();
        androidAutoEntity.reset();
    }
}

void WirelessAndroidAutoDevice::connectHandler(const boost::system::error_code& ec)
{
    if(!ec)
    {
        cinfo << "Connected";
        start();
    }
    else
    {
        cerror << "Failed to connect to AA server with error:" << QString::fromStdString(ec.message());
    }
}
