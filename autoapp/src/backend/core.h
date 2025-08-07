#ifndef AUTOAPP_GUICORE_H
#define AUTOAPP_GUICORE_H

#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QEvent>

#include "settings/guistyle.h"
#include "androidAuto/androidAutoService.h"
#include "logging/loggingCategory.h"
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
};

#endif  // AUTOAPP_GUICORE_H