#ifndef AUTOAPP_LOGGER_H
#define AUTOAPP_LOGGER_H

#include <QDir>
#include <QFile>

#include "../settings/settingsManager.h"
#include "loggingCategory.h"

class Logger {
    inline static const QLoggingCategory category = QLoggingCategory("LOGGER");
    inline static int LOG_ROTATION;
    inline static int LOG_RETENTION;
    inline static QString LOG_FILES_DIR;
    inline static int LOG_LEVEL;
    inline static QDir logsDir;
    inline static QFile logFile;

public:
    static void loadConfig(SettingsManager *settings);
    static void openLogFile();
    static void loggerHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};

#endif  // AUTOAPP_LOGGER_H
