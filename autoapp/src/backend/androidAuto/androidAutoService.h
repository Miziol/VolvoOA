#ifndef AUTOAPP_ANDROIDAUTOSERVICE_H
#define AUTOAPP_ANDROIDAUTOSERVICE_H

#include <libusb-1.0/libusb.h>

#include <QObject>

#include "../logging/loggingCategory.h"
#include "../settingsManager.h"
#include "androidAutoDevice.h"
#include "f1x/openauto/autoapp/Service/AndroidAutoEntityFactory.hpp"
#include "f1x/openauto/autoapp/Service/ServiceFactory.hpp"

class AndroidAutoService : public QObject {
    Q_OBJECT

signals:

public:
    AndroidAutoService(SettingsManager &new_settings, boost::asio::io_service &new_ioService);
    ~AndroidAutoService();

public slots:
    void addUSBDevice(libusb_context *context, libusb_device *device);
    void removeDevice(libusb_device *device);
    void addNetworkDevice();  // TCP
    void clearCurrentAADevice();

    void startIOServiceWorkers(boost::asio::io_service &ioService, std::vector<std::thread> &threadPool);
    void createFactories(QObject *qmlRootObject);

private:
    QLoggingCategory category;
    SettingsManager &settingsManager;

    boost::asio::io_service &ioService;
    f1x::openauto::autoapp::service::ServiceFactory *serviceFactory;
    f1x::openauto::autoapp::service::AndroidAutoEntityFactory *androidAutoEntityFactory;

    AndroidAutoDevice *aaDevice;
};

#endif  // AUTOAPP_ANDROIDAUTOSERVICE_H
