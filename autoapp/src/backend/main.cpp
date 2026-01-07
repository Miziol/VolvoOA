#include <QCursor>
#include <QIcon>
#include <QtGui/QGuiApplication>

#include "core.h"
#include "logging/logger.h"
#include "logging/loggingCategory.h"
#include "settings/settingsManager.h"

const QString applicationName = "AUTOAPP";

int main(int argc, char *argv[]) {
    // Open settings
    SettingsManager::defineSettings(applicationName);
    SettingsManager settings;

    // Start custom logging
    qInstallMessageHandler(Logger::loggerHandler);
    Logger::loadConfig(&settings);
    Logger::openLogFile();

    // Create app
    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/icons/logo.svg"));
    app.setApplicationName(applicationName);
    app.setQuitOnLastWindowClosed(true);
    if (!settings.getShowCursor()) {
        QGuiApplication::setOverrideCursor(QCursor(Qt::BlankCursor));
    }

    // Create modules
    AppCore appCore(settings);

    QLoggingCategory category = QLoggingCategory("MAIN");
    cinfo << "App started";

    // Run app loop
    app.exec();

    /* TODO
    *
    aasdk::usb::USBWrapper usbWrapper(usbContext);
    aasdk::usb::AccessoryModeQueryFactory queryFactory(usbWrapper, ioService);
    aasdk::usb::AccessoryModeQueryChainFactory queryChainFactory(usbWrapper, ioService, queryFactory);
    */

    /* TODO
    *
    auto usbHub(std::make_shared<aasdk::usb::USBHub>(usbWrapper, ioService, queryChainFactory));
    auto connectedAccessoriesEnumerator(std::make_shared<aasdk::usb::ConnectedAccessoriesEnumerator>(usbWrapper,
    ioService, queryChainFactory)); auto app = std::make_shared<autoapp::App>(ioService, usbWrapper, tcpWrapper,
    androidAutoEntityFactory, std::move(usbHub), std::move(connectedAccessoriesEnumerator));
    */
}
