#include "core.h"

#include <libusb-1.0/libusb.h>

#include <QGuiApplication>
#include <QVideoWidget>

#include "androidAuto/androidAutoDevice.h"

AppCore::AppCore(SettingsManager &new_settings)
    : category("GUI CORE"),
      settings(new_settings),
      androidAutoService(new_settings, ioService),
      work(ioService) /*,
       qmlStyle(settings)*/
{
    qmlRootContext = qmlEngine.rootContext();
    qmlRootContext->setContextProperty("core", this);
    // qmlRootContext->setContextProperty("guiStyle", &qmlStyle);
    qmlRootContext->setContextProperty("settingsManager", &settings);
    qmlRootContext->setContextProperty("aaService", &androidAutoService);

    cinfo << "Base QML contexts set";

    qmlEngine.load("qrc:/main.qml");
    qmlRootObject = qmlEngine.rootObjects()[0];

    cinfo << "QML loaded";

    usbService.startUSBWorkers(ioService, threadPool);
    androidAutoService.startIOServiceWorkers(ioService, threadPool);
    androidAutoService.createFactories(qmlRootObject->findChild<QVideoWidget *>("aaVideoOutput"));

    cinfo << "Workers and factories created";

    qRegisterMetaType<libusb_context *>("libusb_context *");
    qRegisterMetaType<libusb_device *>("libusb_device *");

    connect(&usbService, &UsbService::newAndroidAutoDevice, &androidAutoService,
            &AndroidAutoService::addDevice);
    // qmlStyle.changeTextSize(GuiStyle::textSizeType::LARGE);
    // qmlStyle.changeDarkLightMode();
}

AppCore::~AppCore() {
    std::for_each(threadPool.begin(), threadPool.end(), std::bind(&std::thread::join, std::placeholders::_1));
}
