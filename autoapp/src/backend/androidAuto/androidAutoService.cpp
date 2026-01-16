#include "androidAutoService.h"

#include "androidAutoDevice.h"

AndroidAutoService::AndroidAutoService(SettingsManager &new_settings, boost::asio::io_service &new_ioService)
    : category("ANDROID AUTO SERVICE"), settingsManager(new_settings), ioService(new_ioService), aaDevice(nullptr) {}

AndroidAutoService::~AndroidAutoService() {}

void AndroidAutoService::addUSBDevice(libusb_context *context, libusb_device *device) {
    if (aaDevice == nullptr) {
        cinfo << "New AA device start processing";
        aaDevice = new AndroidAutoDevice(this, context, device, ioService, *androidAutoEntityFactory);
    } else {
        cwarning << "Android Auto entity already exist. AA device ignored";
    }
}

void AndroidAutoService::removeDevice(libusb_device *device) {
    if (device == aaDevice->getDevice()) {
        cinfo << "Stoping AA device";
        aaDevice->deleteLater();
        aaDevice = nullptr;
    }
}

void AndroidAutoService::addNetworkDevice() {
    ;  // TODO
}

void AndroidAutoService::startIOServiceWorkers(boost::asio::io_service &ioService,
                                               std::vector<std::thread> &threadPool) {
    auto ioServiceWorker = [&ioService]() { ioService.run(); };

    threadPool.emplace_back(ioServiceWorker);
    threadPool.emplace_back(ioServiceWorker);
    threadPool.emplace_back(ioServiceWorker);
    threadPool.emplace_back(ioServiceWorker);
}

void AndroidAutoService::createFactories(QObject *qmlRootObject) {
    serviceFactory = new f1x::openauto::autoapp::service::ServiceFactory(ioService, settingsManager, qmlRootObject);
    androidAutoEntityFactory = new f1x::openauto::autoapp::service::AndroidAutoEntityFactory(
        this, ioService, settingsManager, *serviceFactory);
}

void AndroidAutoService::onAndroidAutoQuit() {
    aaDevice->deleteLater();
    aaDevice = nullptr;
}

void AndroidAutoService::setFocusOnAA(bool focus) {
    emit focusOnAA(focus);
}
