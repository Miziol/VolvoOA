#include "androidAutoDevice.h"

#include <iostream>

#include "f1x/aasdk/USB/AOAPDevice.hpp"

AndroidAutoDevice::AndroidAutoDevice(
    libusb_context *context,
    libusb_device *new_device,
    libusb_device_descriptor new_descriptor,
    boost::asio::io_service &new_ioService,
    f1x::openauto::autoapp::service::AndroidAutoEntityFactory &new_androidAutoEntityFactory)
    : category("ANDROID AUTO DEVICE"),
      device(new_device),
      descriptor(new_descriptor),
      usbWrapper(f1x::aasdk::usb::USBWrapper(context)),
      ioService(new_ioService),
      androidAutoEntityFactory(new_androidAutoEntityFactory), androidAutoEntity(nullptr) {
    open();
    start();
}

AndroidAutoDevice::~AndroidAutoDevice() {
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

void AndroidAutoDevice::stop() {}

void AndroidAutoDevice::onAndroidAutoQuit() {
    qInfo() << "AndroidAutoQuit";
}

QString AndroidAutoDevice::getName() {
    int maxSize = 128;
    unsigned char manufacturer[maxSize];
    unsigned char product[maxSize];
    libusb_get_string_descriptor_ascii(handle, descriptor.iManufacturer, &manufacturer[0], maxSize);
    libusb_get_string_descriptor_ascii(handle, descriptor.iProduct, &product[0], maxSize);

    return QString(QByteArray(reinterpret_cast<char *>(manufacturer))) +
           QString(QByteArray(reinterpret_cast<char *>(product)));
}
