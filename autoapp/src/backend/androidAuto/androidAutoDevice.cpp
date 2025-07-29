#include "androidAutoDevice.h"

#include <iostream>

#include "f1x/aasdk/USB/AOAPDevice.hpp"

AndroidAutoDevice::AndroidAutoDevice(
    libusb_context *context,
    libusb_device *new_device,
    boost::asio::io_service &new_ioService,
    f1x::openauto::autoapp::service::AndroidAutoEntityFactory &new_androidAutoEntityFactory)
    : category("ANDROID AUTO DEVICE"),
      device(new_device),
      usbWrapper(f1x::aasdk::usb::USBWrapper(context)),
      ioService(new_ioService),
      androidAutoEntityFactory(new_androidAutoEntityFactory),
      androidAutoEntity(nullptr),
      countOnExit(0) {
    open();
    start();
}

AndroidAutoDevice::~AndroidAutoDevice() {
    stop();
    close();
}

void AndroidAutoDevice::open() {
    int result = libusb_open(device, &handle);

    if (result != 0) {
        cerror << "Error opening AA device: " << libusb_error_name(result);
    } else {
        cinfo << "Opened AA device";
    }
}

void AndroidAutoDevice::close() {
    libusb_close(handle);
}

void AndroidAutoDevice::start() {
    if (androidAutoEntity != nullptr)
        return;

    cinfo << "Starting AA entity";

    auto aoapDevice(f1x::aasdk::usb::AOAPDevice::create(usbWrapper, ioService, handle));
    androidAutoEntity = androidAutoEntityFactory.create(std::move(aoapDevice));
    androidAutoEntity->start(*this);

    cinfo << "Started AA entity";
}

void AndroidAutoDevice::stop() {
    if (androidAutoEntity != nullptr)
        androidAutoEntity->stop();
}

void AndroidAutoDevice::onAndroidAutoQuit() {
    qWarning() << "AndroidAutoQuit";
    if (countOnExit > 0) {
        androidAutoEntity.reset();
        emit deleteMe();
    }
    countOnExit++;
}

libusb_device *AndroidAutoDevice::getDevice() {
    return device;
}
