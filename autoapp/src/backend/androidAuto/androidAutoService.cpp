#include "androidAutoService.h"

#include "androidAutoDevice.h"

AndroidAutoService::AndroidAutoService() : category("ANDROID AUTO SERVICE") {}

AndroidAutoService::~AndroidAutoService() {}

void AndroidAutoService::addDevice(libusb_device *device, libusb_device_descriptor descriptor) {
    AAdevices.append(new AndroidAutoDevice(device, descriptor));

    cinfo << "Adding device";
    emit aaDevicesChanged();
}
