#include "usbDevice.h"

#include <boost/asio/posix/descriptor.hpp>

UsbDevice::UsbDevice(libusb_device *new_device) : category("USB DEVICE"), device(new_device), handle(nullptr) {}

UsbDevice::~UsbDevice() {
    close();
}

void UsbDevice::open() {
    int result = libusb_open(device, &handle);

    if (result != 0) {
        cerror << "Error opening USB device: " << libusb_error_name(result);
    } else {
        cinfo << "Opened USB device";
    }
}

bool UsbDevice::isOpen() {
    return handle != nullptr;
}

void UsbDevice::close() {
    libusb_close(handle);
    handle = nullptr;
}

QString UsbDevice::getDeviceName() {
    int bufferLength = 128, manufacturerLength, priductLength;
    unsigned char manufacturer[bufferLength], product[bufferLength];

    libusb_device_descriptor descriptor;
    int result = libusb_get_device_descriptor(device, &descriptor);
    if (result != 0) {
        cerror << "Error getting device descriptor: " << libusb_error_name(result);
    }

    manufacturerLength =
        libusb_get_string_descriptor_ascii(handle, descriptor.iManufacturer, manufacturer, bufferLength);
    if (result < 0) {
        cerror << "Error getting device manufacturer: " << libusb_error_name(result);
    }

    priductLength = libusb_get_string_descriptor_ascii(handle, descriptor.iProduct, product, bufferLength);
    if (result < 0) {
        cerror << "Error getting device product: " << libusb_error_name(result);
    }

    return QString::fromLatin1(reinterpret_cast<const char *>(manufacturer), manufacturerLength) + " " +
           QString::fromLatin1(reinterpret_cast<const char *>(product), priductLength);
}

void UsbDevice::tryToStartAndroidAutoServer() {
    queue.append(UsbDeviceStringRequestType{
        "Android Auto", SendStringType::DESCRIPTION,
        &UsbDevice::sendNextElementFromQueue_cb
    });
    queue.append(
        UsbDeviceStringRequestType{"Android", SendStringType::MANUFACTURER, &UsbDevice::sendNextElementFromQueue_cb});
    queue.append(
        UsbDeviceStringRequestType{"Android Auto", SendStringType::MODEL, &UsbDevice::sendNextElementFromQueue_cb});
    queue.append(
        UsbDeviceStringRequestType{"HU-AAAAAA001", SendStringType::SERIAL, &UsbDevice::sendNextElementFromQueue_cb});
    queue.append(UsbDeviceStringRequestType{
        "https://github.com/Miziol", SendStringType::URI,
        &UsbDevice::sendNextElementFromQueue_cb
    });
    queue.append(UsbDeviceStringRequestType{"1.0.0", SendStringType::VERSION, &UsbDevice::sendNextElementFromQueue_cb});

    cinfo << "Trying to start android-auto-server";

    requestProtocolVersion();
}

void UsbDevice::sendString(std::string message,
                           SendStringType type,
                           void (UsbDevice::*callbackFunction)(libusb_transfer *)) {
    buffer.clear();
    buffer.resize(8);
    buffer.insert(buffer.end(), message.begin(), message.end());
    buffer.push_back('\0');
    libusb_fill_control_setup(&buffer[0], LIBUSB_ENDPOINT_OUT | USB_TYPE_VENDOR, ACC_REQ_SEND_STRING, 0,
                              static_cast<uint16_t>(type), buffer.size() - 8);

    UsbDeviceCallbackStruct *user_data = new (UsbDeviceCallbackStruct){this, callbackFunction};
    startTransfer(user_data);
}

void UsbDevice::startTransfer(UsbDeviceCallbackStruct *user_data) {
    libusb_transfer *transfer = libusb_alloc_transfer(0);

    libusb_fill_control_transfer(transfer, handle, &buffer[0], transfer_cb, user_data, 1000);

    if (libusb_submit_transfer(transfer) != 0) {
        clearTransfer(transfer);
    }
}

void UsbDevice::clearTransfer(libusb_transfer *transfer) {
    delete static_cast<UsbDeviceCallbackStruct *>(transfer->user_data);
    libusb_free_transfer(transfer);
}

void UsbDevice::requestProtocolVersion() {
    int sizeOfProtocolVersion = sizeof(uint16_t);
    buffer.resize(8 + sizeOfProtocolVersion);
    libusb_fill_control_setup(&buffer[0], LIBUSB_ENDPOINT_IN | USB_TYPE_VENDOR, ACC_REQ_GET_PROTOCOL, 0, 0,
                              sizeOfProtocolVersion);

    UsbDeviceCallbackStruct *user_data = new (UsbDeviceCallbackStruct){this, &UsbDevice::requestProtocol_cb};
    startTransfer(user_data);
}

void UsbDevice::requestProtocol_cb(libusb_transfer *transfer) {
    sendNextElementFromQueue();

    clearTransfer(transfer);
}

void UsbDevice::sendNextElementFromQueue() {
    UsbDeviceStringRequestType request = queue.dequeue();
    sendString(request.message, request.type, request.callbackFunction);
}

void UsbDevice::sendNextElementFromQueue_cb(libusb_transfer *transfer) {
    if (queue.empty()) {
        sendStart();
    } else {
        sendNextElementFromQueue();
    }

    clearTransfer(transfer);
}

void UsbDevice::sendStart() {
    buffer.resize(8);
    libusb_fill_control_setup(&buffer[0], LIBUSB_ENDPOINT_OUT | USB_TYPE_VENDOR, ACC_REQ_START, 0, 0, 0);

    UsbDeviceCallbackStruct *user_data = new (UsbDeviceCallbackStruct){this, &UsbDevice::sendStart_cb};
    startTransfer(user_data);
}

void UsbDevice::sendStart_cb(libusb_transfer *transfer) {
    cinfo << "Start configuration send successfully";

    clearTransfer(transfer);
}

void UsbDevice::transfer_cb(libusb_transfer *transfer) {
    UsbDeviceCallbackStruct *user_data = static_cast<UsbDeviceCallbackStruct *>(transfer->user_data);

    (user_data->object->*user_data->UsbDeviceCallbackStruct::callbackFunction)(transfer);
}
