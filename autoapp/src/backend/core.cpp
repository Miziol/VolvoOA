#include "core.h"

#include <libusb.h>

#include <QGuiApplication>

#include "androidAuto/androidAutoDevice.h"

AppCore::AppCore(SettingsManager *new_settings)
    : category("GUI CORE"),
      settings(new_settings) /*,
       qmlStyle(settings)*/
{
    qmlRootContext = qmlEngine.rootContext();
    qmlRootContext->setContextProperty("core", this);
    // qmlRootContext->setContextProperty("guiStyle", &qmlStyle);
    qmlRootContext->setContextProperty("settingsManager", settings);
    qmlRootContext->setContextProperty("aaService", &androidAutoService);

    cinfo << "Base QML contexts set";

    qmlEngine.load("qrc:/main.qml");
    qmlRootObject = qmlEngine.rootObjects()[0];

    cinfo << "QML loaded";

    connect(&usbService, &UsbService::newAndroidAutoDevice, &androidAutoService, &AndroidAutoService::addDevice);
    // qmlStyle.changeTextSize(GuiStyle::textSizeType::LARGE);
    // qmlStyle.changeDarkLightMode();
}

AppCore::~AppCore() {}
