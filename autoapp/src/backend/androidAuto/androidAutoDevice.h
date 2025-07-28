#ifndef AUTOAPP_ANDROIDAUTODEVICE_H
#define AUTOAPP_ANDROIDAUTODEVICE_H

#include <libusb.h>

#include <QAudioFormat>
#include <QObject>
#include <QTimer>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
#include <QtNetwork/QSslSocket>

#include "../../external/openauto-with-aasdk/openauto/include/f1x/openauto/autoapp/Service/AndroidAutoEntity.hpp"
#include "../logging/loggingCategory.h"
#include "f1x/aasdk/USB/USBWrapper.hpp"
#include "f1x/openauto/autoapp/Service/AndroidAutoEntityFactory.hpp"

class AndroidAutoDevice : public QObject, public f1x::openauto::autoapp::service::IAndroidAutoEntityEventHandler {
    Q_OBJECT

public:
    AndroidAutoDevice(libusb_context *context,
                      libusb_device *new_device,
                      boost::asio::io_service &new_ioService,
                      f1x::openauto::autoapp::service::AndroidAutoEntityFactory &new_androidAutoEntityFactory);
    ~AndroidAutoDevice();

private:
    QLoggingCategory category;

    f1x::aasdk::usb::USBWrapper usbWrapper;
    boost::asio::io_service &ioService;
    f1x::openauto::autoapp::service::AndroidAutoEntityFactory &androidAutoEntityFactory;

    libusb_device *device;
    libusb_device_handle *handle;
    f1x::openauto::autoapp::service::IAndroidAutoEntity::Pointer androidAutoEntity;

public slots:
    void open();
    void close();
    void start();
    void stop();
    void onAndroidAutoQuit() override;

private:
};

#endif  // AUTOAPP_ANDROIDAUTODEVICE_H
