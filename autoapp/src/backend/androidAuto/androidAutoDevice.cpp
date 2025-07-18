#include "androidAutoDevice.h"

#include <iostream>

AndroidAutoDevice::AndroidAutoDevice(libusb_device *new_device, libusb_device_descriptor new_descriptor)
    : category("ANDROID AUTO DEVICE"), device(new_device), descriptor(new_descriptor) {
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
    //androidAutoEntity = new f1x::openauto::autoapp::service::AndroidAutoEntity(); // TODO
}

void AndroidAutoDevice::stop() {}

QString AndroidAutoDevice::getName() {
    int maxSize = 128;
    unsigned char manufacturer[maxSize];
    unsigned char product[maxSize];
    libusb_get_string_descriptor_ascii(handle, descriptor.iManufacturer, &manufacturer[0], maxSize);
    libusb_get_string_descriptor_ascii(handle, descriptor.iProduct, &product[0], maxSize);

    return QString(QByteArray(reinterpret_cast<char *>(manufacturer))) +
           QString(QByteArray(reinterpret_cast<char *>(product)));
}

void AndroidAutoDevice::extractEndpointAddresses() {
    libusb_config_descriptor *config_descriptor;

    if (libusb_get_config_descriptor(device, 0, &config_descriptor) != 0) {
        cerror << "Failed to get AA device descriptor";
        return;
    }

    if (config_descriptor->bNumInterfaces == 0) {
        cerror << "Empty interface list";
        return;
    }

    if (config_descriptor->interface[0].num_altsetting == 0) {
        cerror << "Empty interface descriptor";
        return;
    }

    interface = &(config_descriptor->interface[0].altsetting[0]);

    if (interface->bNumEndpoints < 2) {
        cerror << "Too few endpoints";
        return;
    }

    if ((interface->endpoint[0].bEndpointAddress & LIBUSB_ENDPOINT_DIR_MASK) == LIBUSB_ENDPOINT_IN) {
        inputAddress = interface->endpoint[0].bEndpointAddress;
        outputAddress = interface->endpoint[1].bEndpointAddress;
    } else {
        inputAddress = interface->endpoint[1].bEndpointAddress;
        outputAddress = interface->endpoint[0].bEndpointAddress;
    }
}
