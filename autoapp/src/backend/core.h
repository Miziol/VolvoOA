#ifndef AUTOAPP_GUICORE_H
#define AUTOAPP_GUICORE_H

#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>

// #include "../commons/guistyle.h"
#include "androidAuto/androidAutoService.h"
#include "logging/loggingCategory.h"
#include "settingsManager.h"
#include "usb/usbService.h"

class AppCore : public QObject {
    Q_OBJECT

public:
    AppCore(SettingsManager *new_settings);
    ~AppCore();

public slots:

private:
    QLoggingCategory category;

    SettingsManager *settings;
    // GuiStyle qmlStyle;

    AndroidAutoService androidAutoService;
    UsbService usbService;

    QQmlApplicationEngine qmlEngine;
    QQmlContext *qmlRootContext;
    QObject *qmlRootObject;
};

#endif  // AUTOAPP_GUICORE_H