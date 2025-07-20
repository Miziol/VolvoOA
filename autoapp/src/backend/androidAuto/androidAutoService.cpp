#include "androidAutoService.h"

#include "androidAutoDevice.h"

AndroidAutoService::AndroidAutoService(SettingsManager &new_settings) : category("ANDROID AUTO SERVICE"), settingsManager(new_settings) {
    serviceFactory = new f1x::openauto::autoapp::service::ServiceFactory(ioService, settingsManager);
    androidAutoEntityFactory = new f1x::openauto::autoapp::service::AndroidAutoEntityFactory(ioService, settingsManager, *serviceFactory);
}

AndroidAutoService::~AndroidAutoService() {}

void AndroidAutoService::addDevice(libusb_context *context,
                                   libusb_device *device,
                                   libusb_device_descriptor descriptor) {
    AAdevices.append(new AndroidAutoDevice(context, device, descriptor, ioService, *androidAutoEntityFactory));

    cinfo << "Adding device";
    emit aaDevicesChanged();
}
