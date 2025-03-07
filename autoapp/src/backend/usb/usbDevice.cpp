#include "usbDevice.h"

UsbDevice::UsbDevice(QObject *new_parent, libusb_device *new_device)
    : QObject(new_parent), category("USB DEVICE"), device(new_device), m_isOpen(false) {}

UsbDevice::~UsbDevice() {
    close();
}

void UsbDevice::open() {
    int result = libusb_open(device, &handle);

    if (result != 0) {
        cerror << "Error opening USB device: " << libusb_error_name(result);
    } else {
        m_isOpen = true;
        cinfo << "Opened USB device";
    }
}

bool UsbDevice::isOpen() {
    return m_isOpen;
}

void UsbDevice::close() {
    libusb_close(handle);
    handle = nullptr;
    m_isOpen = false;
}

void UsbDevice::tryToStartAndroidAutoServer() {
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
    m_transferInProgress = true;

    libusb_transfer *transfer;

    transfer = libusb_alloc_transfer(0);

    libusb_fill_control_transfer(transfer, handle, &buffer[0], transfer_cb, user_data, 1000);

    if (libusb_submit_transfer(transfer) != 0) {
        clearTransfer(transfer);
    }
}

void UsbDevice::clearTransfer(libusb_transfer *transfer) {
    delete (UsbDeviceCallbackStruct *)(transfer->user_data);
    libusb_free_transfer(transfer);
    m_transferInProgress = false;
}

void UsbDevice::requestProtocolVersion() {
    cinfo << "Requesting protocol version";

    int sizeOfProtocolVersion = sizeof(uint16_t);
    buffer.resize(8 + sizeOfProtocolVersion);
    libusb_fill_control_setup(&buffer[0], LIBUSB_ENDPOINT_IN | USB_TYPE_VENDOR, ACC_REQ_GET_PROTOCOL, 0, 0,
                              sizeOfProtocolVersion);

    UsbDeviceCallbackStruct *user_data = new (UsbDeviceCallbackStruct){this, &UsbDevice::requestProtocol_cb};
    startTransfer(user_data);
}

void UsbDevice::requestProtocol_cb(libusb_transfer *transfer) {
    cinfo << "Protocol version" << static_cast<uint16_t>(transfer->buffer[8]);

    sendDescription();  // TODO create queue

    clearTransfer(transfer);
}

void UsbDevice::sendDescription() {
    cinfo << "Sending description";
    sendString("Android Auto", SendStringType::DESCRIPTION, &UsbDevice::sendDescription_cb);
}

void UsbDevice::sendDescription_cb(libusb_transfer *transfer) {
    cinfo << "Description sent";

    sendManufacturer();

    clearTransfer(transfer);
}

void UsbDevice::sendManufacturer() {
    cinfo << "Sending manufacturer";
    sendString("Android", SendStringType::MANUFACTURER, &UsbDevice::sendManufacturer_cb);
}

void UsbDevice::sendManufacturer_cb(libusb_transfer *transfer) {
    cinfo << "Manufacturer sent";

    sendModel();

    clearTransfer(transfer);
}

void UsbDevice::sendModel() {
    cinfo << "Sending model";
    sendString("Android Auto", SendStringType::MODEL, &UsbDevice::sendModel_cb);
}

void UsbDevice::sendModel_cb(libusb_transfer *transfer) {
    cinfo << "Model sended";

    sendSerial();

    clearTransfer(transfer);
}

void UsbDevice::sendSerial() {
    cinfo << "Sending serial";
    sendString("HU-AAAAAA001", SendStringType::SERIAL, &UsbDevice::sendSerial_cb);
}

void UsbDevice::sendSerial_cb(libusb_transfer *transfer) {
    cinfo << "Serial sent";

    sendUri();

    clearTransfer(transfer);
}

void UsbDevice::sendUri() {
    cinfo << "Sending URI";
    sendString("https://github.com/Miziol", SendStringType::URI, &UsbDevice::sendUri_cb);
}

void UsbDevice::sendUri_cb(libusb_transfer *transfer) {
    cinfo << "Uri sent";

    sendVersion();

    clearTransfer(transfer);
}

void UsbDevice::sendVersion() {
    cinfo << "Sending version";
    sendString("1.0.0", SendStringType::VERSION, &UsbDevice::sendVersion_cb);
}

void UsbDevice::sendVersion_cb(libusb_transfer *transfer) {
    cinfo << "Version sent";

    sendStart();

    clearTransfer(transfer);
}

void UsbDevice::sendStart() {
    cinfo << "Sending start";

    buffer.resize(8);
    libusb_fill_control_setup(&buffer[0], LIBUSB_ENDPOINT_OUT | USB_TYPE_VENDOR, ACC_REQ_START, 0, 0, 0);

    UsbDeviceCallbackStruct *user_data = new (UsbDeviceCallbackStruct){this, &UsbDevice::sendStart_cb};
    startTransfer(user_data);
}

void UsbDevice::sendStart_cb(libusb_transfer *transfer) {
    cinfo << "Start sent";

    clearTransfer(transfer);
}

void UsbDevice::transfer_cb(libusb_transfer *transfer) {
    UsbDeviceCallbackStruct *user_data = (UsbDeviceCallbackStruct *)(transfer->user_data);

    (user_data->object->*user_data->UsbDeviceCallbackStruct::callbackFunction)(transfer);
}
