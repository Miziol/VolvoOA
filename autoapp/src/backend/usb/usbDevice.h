#ifndef AUTOAPP_USBDEVICE_H
#define AUTOAPP_USBDEVICE_H

#include <libusb-1.0/libusb.h>

#include <QObject>
#include <QQueue>

#include "../androidAuto/aaDeviceEntry.h"
#include "../logging/loggingCategory.h"

struct UsbDeviceCallbackStruct;

struct UsbDeviceStringRequestType;

enum class SendStringType { MANUFACTURER, MODEL, DESCRIPTION, VERSION, URI, SERIAL };

class UsbDevice : public AaDeviceEntry {
    Q_OBJECT

public:
    UsbDevice(libusb_device *new_device);
    ~UsbDevice();

public:
    // Acquires the transport channel (the libusb handle). Transport-internal, not part of the
    // activate()/deactivate() row lifecycle: UsbService opens it up-front, at discovery.
    void open();

    QString displayName() const override;
    Type type() const override { return Type::Usb; }
    // Engaged = the libusb handle is open, which UsbService does at discovery - so a listed USB
    // entry is "active" before the user taps it. Not a user-intent flag.
    bool isActive() const override;

public slots:
    // Twin of BluetoothDevice::activate(): the list-entry "start" action. Kicks off the AOAP
    // accessory handshake; the phone re-enumerates as an AA device and AndroidAutoService
    // then picks it up via UsbService::newAndroidAutoDevice.
    void activate() override;
    void deactivate() override;

private:
    QLoggingCategory category;

    libusb_device *device;
    libusb_device_handle *handle;
    std::vector<uint8_t> buffer;

    QQueue<UsbDeviceStringRequestType> queue;

    static constexpr uint32_t USB_TYPE_VENDOR = 0x40;
    static constexpr uint32_t ACC_REQ_GET_PROTOCOL = 51;
    static constexpr uint32_t ACC_REQ_SEND_STRING = 52;
    static constexpr uint32_t ACC_REQ_START = 53;

    void sendString(std::string message, SendStringType type, void (UsbDevice::*callbackFunction)(libusb_transfer *));
    void startTransfer(UsbDeviceCallbackStruct *user_data);
    void clearTransfer(libusb_transfer *transfer);

    void requestProtocolVersion();
    void requestProtocol_cb(libusb_transfer *transfer);
    void sendNextElementFromQueue();
    void sendNextElementFromQueue_cb(libusb_transfer *transfer);
    void sendStart();
    void sendStart_cb(libusb_transfer *transfer);

    static void transfer_cb(libusb_transfer *transfer);
};

struct UsbDeviceCallbackStruct {
    UsbDevice *object;
    void (UsbDevice::*callbackFunction)(libusb_transfer *);
};

struct UsbDeviceStringRequestType {
    std::string message;
    SendStringType type;
    void (UsbDevice::*callbackFunction)(libusb_transfer *);
};

#endif  // AUTOAPP_USBDEVICE_H
