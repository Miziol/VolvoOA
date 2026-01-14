#include <QCursor>
#include <QIcon>
#include <QtGui/QGuiApplication>
#include <QSocketNotifier>
#include <csignal>
#include <sys/socket.h>
#include <unistd.h>


#include "core.h"
#include "logging/logger.h"
#include "logging/loggingCategory.h"
#include "settings/settingsManager.h"

static int sigTermFd[2];
const QString applicationName = "AUTOAPP";

static void handleSigTerm(int)
{
    char a = 1;
    ::write(sigTermFd[0], &a, sizeof(a));
}

int main(int argc, char *argv[]) {
    // Closing logic
    if (::socketpair(AF_UNIX, SOCK_STREAM, 0, sigTermFd)) {
        qFatal("Nie można utworzyć socketpair dla handlera sygnałów");
    }

    struct sigaction term;
    term.sa_handler = handleSigTerm;
    sigemptyset(&term.sa_mask);
    term.sa_flags = SA_RESTART;
    if (sigaction(SIGTERM, &term, nullptr) > 0) return 1;
    if (sigaction(SIGINT, &term, nullptr) > 0) return 1;

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

    // Connect to close signal
    QSocketNotifier closingSocket(sigTermFd[1], QSocketNotifier::Read);
    QObject::connect(&closingSocket, &QSocketNotifier::activated, [&app]() {
        app.quit();
    });

    // Create modules
    AppCore appCore(settings);

    QLoggingCategory category = QLoggingCategory("MAIN");
    cinfo << "App started";

    // Run app loop
    app.exec();

    cinfo << "Application loop stop. Application closing";
}