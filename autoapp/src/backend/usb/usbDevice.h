#ifndef AUTOAPP_USBDEVICE_H
#define AUTOAPP_USBDEVICE_H

#include <libusb-1.0/libusb.h>

#include <QObject>

#include "../logging/loggingCategory.h"

struct UsbDeviceCallbackStruct;

enum class SendStringType { MANUFACTURER, MODEL, DESCRIPTION, VERSION, URI, SERIAL };

class UsbDevice : public QObject {
    Q_OBJECT

public:
    UsbDevice(libusb_device *new_device);
    ~UsbDevice();

public:
    void open();
    bool isOpen();
    void close();
    void tryToStartAndroidAutoServer();

private:
    bool m_isOpen, m_transferInProgress;

    QLoggingCategory category;

    libusb_device *device;
    libusb_device_handle *handle;
    std::vector<uint8_t> buffer;

    static constexpr uint32_t USB_TYPE_VENDOR = 0x40;
    static constexpr uint32_t ACC_REQ_GET_PROTOCOL = 51;
    static constexpr uint32_t ACC_REQ_SEND_STRING = 52;
    static constexpr uint32_t ACC_REQ_START = 53;

    void sendString(std::string message, SendStringType type, void (UsbDevice::*callbackFunction)(libusb_transfer *));
    void startTransfer(UsbDeviceCallbackStruct *user_data);
    void clearTransfer(libusb_transfer *transfer);

    void requestProtocolVersion();
    void requestProtocol_cb(libusb_transfer *transfer);
    void sendDescription();
    void sendDescription_cb(libusb_transfer *transfer);
    void sendManufacturer();
    void sendManufacturer_cb(libusb_transfer *transfer);
    void sendModel();
    void sendModel_cb(libusb_transfer *transfer);
    void sendSerial();
    void sendSerial_cb(libusb_transfer *transfer);
    void sendUri();
    void sendUri_cb(libusb_transfer *transfer);
    void sendVersion();
    void sendVersion_cb(libusb_transfer *transfer);
    void sendStart();
    void sendStart_cb(libusb_transfer *transfer);

    static void transfer_cb(libusb_transfer *transfer);
};

struct UsbDeviceCallbackStruct {
    UsbDevice *object;
    void (UsbDevice::*callbackFunction)(libusb_transfer *);
};

#endif  // AUTOAPP_USBDEVICE_H
