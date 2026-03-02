#ifndef AUTOAPP_USBANDROIDAUTODEVICE_H
#define AUTOAPP_USBANDROIDAUTODEVICE_H

#include <libusb.h>

#include "androidAutoDevice.h"
#include "f1x/aasdk/USB/USBWrapper.hpp"
#include "f1x/openauto/autoapp/Service/AndroidAutoEntity.hpp"
#include "f1x/openauto/autoapp/Service/AndroidAutoEntityFactory.hpp"

class UsbAndroidAutoDevice : public AndroidAutoDevice {
    Q_OBJECT

signals:

public:
    UsbAndroidAutoDevice(QObject *parent,
                         libusb_context *context,
                         libusb_device *new_device,
                         boost::asio::io_service &new_ioService,
                         f1x::openauto::autoapp::service::AndroidAutoEntityFactory &new_androidAutoEntityFactory);
    ~UsbAndroidAutoDevice();

private:
    f1x::aasdk::usb::USBWrapper usbWrapper;

    libusb_device *device;
    libusb_device_handle *handle;

public slots:
    void open() override;
    void close() override;
    void start() override;
    void stop() override;

    libusb_device *getDevice();

private:
};

#endif  // AUTOAPP_USBANDROIDAUTODEVICE_H
