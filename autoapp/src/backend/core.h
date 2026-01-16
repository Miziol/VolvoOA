#ifndef AUTOAPP_GUICORE_H
#define AUTOAPP_GUICORE_H

#include <QEvent>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "androidAuto/androidAutoService.h"
#include "app/appUpdater.h"
#include "app/systemUpdater.h"
#include "car/CANService.h"
#include "car/arduinoService.h"
#include "logging/loggingCategory.h"
#include "settings/guistyle.h"
#include "settings/settingsManager.h"
#include "usb/usbService.h"

Q_DECLARE_OPAQUE_POINTER(libusb_context *)

class AppCore : public QObject {
    Q_OBJECT

public:
    AppCore(SettingsManager &new_settings);
    ~AppCore();

public slots:
    bool eventFilter(QObject *obj, QEvent *event);

    void shutdownSystem();

    void updateApp();
    void updateSystem();

private:
    QLoggingCategory category;

    SettingsManager &settings;
    GuiStyle qmlStyle;

    QQmlApplicationEngine qmlEngine;
    QQmlContext *qmlRootContext;
    QObject *qmlRootObject;

    boost::asio::io_service ioService;
    boost::asio::io_service::work work;
    std::vector<std::thread> threadPool;
    AndroidAutoService androidAutoService;
    UsbService usbService;
    CANService canbusService;
    ArduinoService arduinoService;

    SystemUpdater systemUpdater;
    AppUpdater appUpdater;
};

#endif  // AUTOAPP_GUICORE_H
