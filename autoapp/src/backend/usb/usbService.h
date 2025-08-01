#ifndef AUTOAPP_USBSERVICE_H
#define AUTOAPP_USBSERVICE_H

#include <libusb-1.0/libusb.h>
#include <sys/time.h>

#include <QObject>
#include <QTimer>
#include <boost/asio/io_service.hpp>
#include <thread>

#include "../logging/loggingCategory.h"
#include "../usb/usbDevice.h"

class UsbService : public QObject {
    Q_OBJECT

signals:
    void usbDevicesChanged();
    void newAndroidAutoDevice(libusb_context *context, libusb_device *device);
    void removeAndroidAutoDevice(libusb_device *device);

public:
    Q_PROPERTY(QList<QObject *> usbDevices MEMBER devices NOTIFY usbDevicesChanged);

private:
    static inline uint16_t VENDOR_GOOGLE_INC = 0x18D1;
    static inline QList<uint16_t> PRODUCT_WITH_ANDROID_AUTO = QList<uint16_t>() << 0x2D00 << 0x2D01 << 0x2D04 << 0x2D05;

public:
    UsbService();
    ~UsbService();

public slots:
    void handleLibUsbEvents();
    void startUSBWorkers(boost::asio::io_service &ioService, std::vector<std::thread> &threadPool);

    void newDevice(libusb_device *device);
    void removeDevice(libusb_device *device);

private:
    QLoggingCategory category;

    timeval libusbEventTimeout{0, 0};
    QTimer libusbTimer;
    libusb_context *usbContext;
    libusb_hotplug_callback_handle callbackHandler;

    QList<QObject *> devices;

    static int usbHotplug_cb(libusb_context *context,
                             libusb_device *device,
                             libusb_hotplug_event event,
                             void *user_data);
};

Q_DECLARE_OPAQUE_POINTER(libusb_device *)

#endif  // AUTOAPP_USBSERVICE_H
