#include "androidAutoService.h"

#include "androidAutoDevice.h"

AndroidAutoService::AndroidAutoService(SettingsManager &new_settings, boost::asio::io_service &new_ioService)
    : category("ANDROID AUTO SERVICE"), settingsManager(new_settings), ioService(new_ioService) {}

AndroidAutoService::~AndroidAutoService() {}

void AndroidAutoService::addDevice(libusb_context *context, libusb_device *device) {
    cinfo << "New AA device start processing";
    AAdevices.append(new AndroidAutoDevice(context, device, ioService, *androidAutoEntityFactory));

    cinfo << "Adding device";
    emit aaDevicesChanged();
}

void AndroidAutoService::startIOServiceWorkers(boost::asio::io_service &ioService,
                                               std::vector<std::thread> &threadPool) {
    auto ioServiceWorker = [&ioService]() { ioService.run(); };

    threadPool.emplace_back(ioServiceWorker);
    threadPool.emplace_back(ioServiceWorker);
    threadPool.emplace_back(ioServiceWorker);
    threadPool.emplace_back(ioServiceWorker);
}

void AndroidAutoService::createFactories(QVideoWidget *widget) {
    serviceFactory = new f1x::openauto::autoapp::service::ServiceFactory(
        ioService, settingsManager);  // TODO  connect GUI to external backend
    androidAutoEntityFactory =
        new f1x::openauto::autoapp::service::AndroidAutoEntityFactory(ioService, settingsManager, *serviceFactory);
}
