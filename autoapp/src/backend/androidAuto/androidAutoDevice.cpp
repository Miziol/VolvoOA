#include "androidAutoDevice.h"

#include <iostream>

AndroidAutoDevice::AndroidAutoDevice(libusb_device *new_device, libusb_device_descriptor new_descriptor, SSL_CTX *ssl_ctx)
    : category("ANDROID AUTO DEVICE"), device(new_device), descriptor(new_descriptor), context(ssl_ctx), maxBufferSize(1024 * 20) {
    open();

    configSSL();

    // MESSENGER CREATE
}

AndroidAutoDevice::~AndroidAutoDevice() {
    close();
}

void AndroidAutoDevice::open() {
    int result;

    result = libusb_open(device, &handle);

    if (result != 0) {
        cerror << "Error opening AA device: " << libusb_error_name(result);
        return;
    }

    if (libusb_get_active_config_descriptor(device, &config_descriptor) != 0) {
        cerror << "Failed to get AA config descriptor";
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
    result = libusb_claim_interface(handle, interface->bInterfaceNumber);

    if (result != 0) {
        cerror << "Error claiming AA interface: " << libusb_error_name(result);
        return;
    }

    extractEndpointAddresses();

    cinfo << "Opened AA device";
}

void AndroidAutoDevice::close() {
    libusb_release_interface(handle, interface->bInterfaceNumber);
    libusb_free_config_descriptor(config_descriptor);
    libusb_close(handle);
}

void AndroidAutoDevice::start() {
    cinfo << "Starting AA device";

    std::vector<uint8_t> versionRequest;
    versionRequest.push_back(MessageType::VersionRequest >> 8);
    versionRequest.push_back(MessageType::VersionRequest);
    versionRequest.push_back(VERSION_MAJOR >> 8);
    versionRequest.push_back(VERSION_MAJOR);
    versionRequest.push_back(VERSION_MINOR >> 8);
    versionRequest.push_back(VERSION_MINOR);
    sendMessageToChannel(
        ChannelID::CONTROL,
        EncryptionType::Plain | MessageTypeFlags::Control | FrameType::Bulk,
        versionRequest
    );


    startListening(ChannelID::CONTROL, interfaceCallback);
    // MESSENGER
}

void AndroidAutoDevice::stop() {
    clearSSL();
}

QString AndroidAutoDevice::getName() {
    return QString::number(descriptor.idVendor) + ":" + QString::number(descriptor.idProduct);
}

void AndroidAutoDevice::sendMessageToChannel(ChannelID channel, uint8_t flags, std::vector<uint8_t> data) {


}

void AndroidAutoDevice::extractEndpointAddresses() {
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

void AndroidAutoDevice::configSSL() {
    // Instance
    sslConnection = SSL_new(context);

    // BIO
    inputBIO = BIO_new(BIO_s_mem());
    outputBIO = BIO_new(BIO_s_mem());
    SSL_set_bio(sslConnection, inputBIO, outputBIO);
    BIO_set_write_buf_size(inputBIO, maxBufferSize);
    BIO_set_write_buf_size(outputBIO, maxBufferSize);

    SSL_set_connect_state(sslConnection);
    SSL_set_verify(sslConnection, SSL_VERIFY_NONE, nullptr);
}

void AndroidAutoDevice::clearSSL() {
    BIO_free(inputBIO);
    BIO_free(outputBIO);
    SSL_free(sslConnection);
}

void AndroidAutoDevice::startListening(ChannelID channelID, void callback(libusb_transfer *)) {
    int result;
    libusb_transfer *transfer = libusb_alloc_transfer(0);
    unsigned char *buffer = new unsigned char[512 * 64];  // Bufor danych

    libusb_fill_bulk_transfer(transfer, handle, inputAddress, buffer, 512 * 64, callback, this, 0);
    result = libusb_submit_transfer(transfer);
    if (result != 0) {
        cerror << "Error submitting transfer: " << libusb_error_name(result);
    }
}

void AndroidAutoDevice::interfaceCallback(libusb_transfer *transfer) {
    qInfo() << "Bulk transfer recived";

    delete transfer->buffer;
    libusb_free_transfer(transfer);
}
