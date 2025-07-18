#ifndef AUTOAPP_ANDROIDAUTODEVICE_H
#define AUTOAPP_ANDROIDAUTODEVICE_H

#include <libusb.h>

#include <QAudioFormat>
#include <QObject>
#include <QTimer>

#include "../../external/openauto-with-aasdk/openauto/include/f1x/openauto/autoapp/Service/AndroidAutoEntity.hpp"

#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
#include <QtNetwork/QSslSocket>

#include "../logging/loggingCategory.h"

class AndroidAutoDevice : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name READ getName CONSTANT)

public:
    AndroidAutoDevice(libusb_device *new_device, libusb_device_descriptor new_descriptor);
    ~AndroidAutoDevice();

private:
    QLoggingCategory category;

    libusb_device *device;
    libusb_device_handle *handle;
    libusb_device_descriptor descriptor;
    const libusb_interface_descriptor *interface;
    uint8_t inputAddress, outputAddress;

    f1x::openauto::autoapp::service::AndroidAutoEntity *androidAutoEntity;

public slots:
    void open();
    void close();
    void start();
    void stop();

    QString getName();

private:
    void extractEndpointAddresses();
};

#endif  // AUTOAPP_ANDROIDAUTODEVICE_H
