#include "core.h"

#include <libusb-1.0/libusb.h>

#include <QGuiApplication>
#include <QKeyEvent>

int AppCore::sigterm[2] = {0, 0};

AppCore::AppCore(SettingsManager &new_settings)
    : category("GUI CORE"),
      settings(new_settings),
      androidAutoService(new_settings, ioService),
      work(ioService),
      qmlStyle(settings) {
    QGuiApplication::instance()->installEventFilter(this);

    systemUpdater.setAutoDelete(false);
    appUpdater.setAutoDelete(false);

    qmlRootContext = qmlEngine.rootContext();
    qmlRootContext->setContextProperty("core", this);
    qmlRootContext->setContextProperty("guiStyle", &qmlStyle);
    qmlRootContext->setContextProperty("settingsManager", &settings);
    qmlRootContext->setContextProperty("usbService", &usbService);
    qmlRootContext->setContextProperty("aaService", &androidAutoService);
    qmlRootContext->setContextProperty("arduinoService", &arduinoService);
    qmlRootContext->setContextProperty("systemUpdater", &systemUpdater);
    qmlRootContext->setContextProperty("appUpdater", &appUpdater);

    cinfo << "Base QML contexts set";

    qmlEngine.load("qrc:/main.qml");
    qmlRootObject = qmlEngine.rootObjects()[0];

    cinfo << "QML loaded";

    usbService.startUSBWorkers(ioService, threadPool);
    androidAutoService.startIOServiceWorkers(ioService, threadPool);
    androidAutoService.createFactories(qmlRootObject);

    cinfo << "Workers and factories created";

    qRegisterMetaType<libusb_context *>("libusb_context *");
    qRegisterMetaType<libusb_device *>("libusb_device *");

    connect(&arduinoService, &ArduinoService::piShutdownRequestReceived, this, &AppCore::shutdownSystem);

    setupUnixSignalHandlers();
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigterm))
        qFatal("Couldn't create TERM socketpair");
    sigtermSocketNotifier = new QSocketNotifier(sigterm[1], QSocketNotifier::Read, this);
    connect(sigtermSocketNotifier, &QSocketNotifier::activated, this, &AppCore::handleSigTerm);

    connect(&usbService, &UsbService::newAndroidAutoDevice, &androidAutoService, &AndroidAutoService::addUSBDevice);
    connect(&usbService, &UsbService::removeAndroidAutoDevice, &androidAutoService, &AndroidAutoService::removeDevice);
}

AppCore::~AppCore() {
    ioService.stop();
    std::for_each(threadPool.begin(), threadPool.end(), std::bind(&std::thread::join, std::placeholders::_1));
}

bool AppCore::eventFilter(QObject *obj, QEvent *event) {
    if (QGuiApplication::focusObject() != nullptr &&
        (event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);

        if (keyEvent->key() == Qt::Key_PageUp) {
            QGuiApplication::sendEvent(QGuiApplication::focusObject(), new QKeyEvent(keyEvent->type(), Qt::Key_Backtab,
                                                                                     Qt::KeyboardModifier::NoModifier));
            return true;
        } else if (keyEvent->key() == Qt::Key_PageDown) {
            QGuiApplication::sendEvent(QGuiApplication::focusObject(),
                                       new QKeyEvent(keyEvent->type(), Qt::Key_Tab, Qt::KeyboardModifier::NoModifier));
            return true;
        }
    }
    return QObject::eventFilter(obj, event);
}

void AppCore::shutdownSystem()
{
    QProcess process;
    process.startCommand("shutdown now -h");
    process.waitForFinished(-1);
}

void AppCore::handleSigTerm()
{
    sigtermSocketNotifier->setEnabled(false);
    char tmp;
    ::read(sigterm[1], &tmp, sizeof(tmp));

    QGuiApplication::quit();

    sigtermSocketNotifier->setEnabled(true);
}

void AppCore::updateApp()
{
    QThreadPool::globalInstance()->start(&appUpdater);
}

void AppCore::updateSystem()
{
    QThreadPool::globalInstance()->start(&systemUpdater);
}

void AppCore::terminateSignalHandler(int signal)
{
    char a = 1;
    ::write(sigterm[0], &a, sizeof(a));
}

int AppCore::setupUnixSignalHandlers()
{
    struct sigaction term;

    term.sa_handler = AppCore::terminateSignalHandler;
    sigemptyset(&term.sa_mask);
    term.sa_flags = 0;
    term.sa_flags |= SA_RESTART;

    if (sigaction(SIGTERM, &term, 0))
        return 1;

    return 0;
}
