#ifndef AUTOAPP_ANDROIDAUTOSERVICE_H
#define AUTOAPP_ANDROIDAUTOSERVICE_H

#include <libusb.h>

#include <QObject>
#include <QStandardItemModel>

#include "../logging/loggingCategory.h"

class AndroidAutoService : public QObject {
    Q_OBJECT

signals:
    void aaDevicesChanged();

public:
    AndroidAutoService();
    ~AndroidAutoService();

    Q_PROPERTY(QList<QObject *> aaDevices MEMBER AAdevices NOTIFY aaDevicesChanged);

public slots:
    void addDevice(libusb_device *device, libusb_device_descriptor descriptor);

private:
    QLoggingCategory category;

    QList<QObject *> AAdevices;
};

#endif  // AUTOAPP_ANDROIDAUTOSERVICE_H
