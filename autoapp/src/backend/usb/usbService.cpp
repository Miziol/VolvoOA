#include "usbService.h"

UsbService::UsbService() : category("USB SERVICE") {
    cdebug << "LIBUSB API version" << LIBUSB_API_VERSION;

    int result;

    result = libusb_init_context(&usbContext, NULL, 0);
    if (result == LIBUSB_SUCCESS) {
        cdebug << "LIBUSB context set";
    } else {
        cerror << "LIBUSB context error";
        return;
    }

    result = libusb_hotplug_register_callback(
        usbContext, LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED | LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT, LIBUSB_HOTPLUG_NO_FLAGS,
        LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY, LIBUSB_HOTPLUG_MATCH_ANY, &UsbService::usbHotplug_cb, this,
        &callbackHandler);
    if (result == LIBUSB_SUCCESS) {
        cdebug << "LIBUSB callback set";
    } else {
        cerror << "LIBUSB callback error";
    }

    connect(&libusbTimer, &QTimer::timeout, this, &UsbService::handleLibUsbEvents);
    libusbTimer.setInterval(1000);
    libusbTimer.start();

    cinfo << "Service started";
}

UsbService::~UsbService() {
    libusb_hotplug_deregister_callback(usbContext, callbackHandler);
    libusb_exit(usbContext);
}

void UsbService::handleLibUsbEvents() {
    libusb_handle_events_timeout(usbContext, &libusbEventTimeout);
}

void UsbService::newDevice(libusb_device *device) {
    qInfo() << device;

    int result;
    libusb_device_descriptor deviceDescriptor;

    result = libusb_get_device_descriptor(device, &deviceDescriptor);
    if (result != LIBUSB_SUCCESS) {
        cerror << "Getting device descriptor error: " << libusb_error_name(result);
    }

    if (deviceDescriptor.idVendor == VENDOR_GOOGLE_INC &&
        PRODUCT_WITH_ANDROID_AUTO.contains(deviceDescriptor.idProduct)) {
        cinfo << "New device with AA inserted";

        emit newAndroidAutoDevice(usbContext, device, deviceDescriptor);
    } else {
        UsbDevice *newDevice = new UsbDevice(this, device);
        devices.append(newDevice);
        newDevice->open();
        newDevice->tryToStartAndroidAutoServer();
    }
}

void UsbService::removeDevice(libusb_device *device) {
    cinfo << "Remove device" << device;
    QMutableListIterator<UsbDevice *> i(devices);
    while (i.hasNext()) {
        UsbDevice *deviceToRemove = i.next();
        i.remove();
        delete deviceToRemove;
    }

    // TODO request to remove AA device
}

int UsbService::usbHotplug_cb(libusb_context *context,
                              libusb_device *device,
                              libusb_hotplug_event event,
                              void *user_data) {
    UsbService *service = reinterpret_cast<UsbService *>(user_data);

    if (event == LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED) {
        service->newDevice(device);
    } else {
        service->removeDevice(device);
    }

    return 0;
}
