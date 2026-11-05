#include "usbAndroidAutoDevice.h"

#include "androidAutoService.h"
#include "f1x/aasdk/USB/AOAPDevice.hpp"

UsbAndroidAutoDevice::UsbAndroidAutoDevice(
    QObject *parent,
    libusb_device *new_device,
    boost::asio::io_context &new_ioService,
    f1x::openauto::autoapp::service::AndroidAutoEntityFactory &new_androidAutoEntityFactory,
    libusb_context *new_context)
    : AndroidAutoDevice(parent, "USB ANDROID AUTO DEVICE", new_ioService,
                        new_androidAutoEntityFactory),
      usbWrapper(new_context),
      device(new_device) {}

UsbAndroidAutoDevice::~UsbAndroidAutoDevice() {
    stop();
    close();
}

void UsbAndroidAutoDevice::open() {
    int result = libusb_open(device, &handle);

    if (result != 0) {
        cerror << "Error opening AA device: " << libusb_error_name(result);
    } else {
        cinfo << "Opened AA device";
    }
}

void UsbAndroidAutoDevice::close() {
    libusb_close(handle);
}

void UsbAndroidAutoDevice::start() {
    if (androidAutoEntity != nullptr)
        return;

    cinfo << "Starting AA entity";

    auto aoapDevice(f1x::aasdk::usb::AOAPDevice::create(usbWrapper, ioService, handle));
    androidAutoEntity = androidAutoEntityFactory.create(std::move(aoapDevice));
    androidAutoEntity->start(*((AndroidAutoService *)(parent())));

    cinfo << "Started AA entity";
}

void UsbAndroidAutoDevice::stop() {
    if (androidAutoEntity != nullptr) {
        androidAutoEntity->stop();
        androidAutoEntity.reset();
    }
}

libusb_device *UsbAndroidAutoDevice::getDevice() {
    return device;
}
