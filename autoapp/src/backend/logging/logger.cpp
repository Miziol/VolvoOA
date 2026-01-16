#include "logger.h"

#include <iostream>

void Logger::loadConfig(SettingsManager *settings) {
    LOG_ROTATION = settings->getLoggerRotation();
    LOG_RETENTION = settings->getLoggerRetention();
    LOG_FILES_DIR = settings->getLoggerPath();
    LOG_LEVEL = settings->getLoggetStdOutputLevel();
}

void Logger::openLogFile() {
    // DIR
    logsDir.setPath(LOG_FILES_DIR);
    if (!logsDir.exists()) {
        if (logsDir.mkpath(LOG_FILES_DIR)) {
            cdebug << "Created logging directory" << logsDir.path();
        } else {
            cwarning << "Logging directory doesn't exist, and folder creation fails";
        }
    }

    // FILE
    logFile.setFileName(LOG_FILES_DIR + "autoapp.log");
    if (!logFile.exists() && logFile.open(QIODeviceBase::NewOnly | QIODeviceBase::Append,
                                          QFileDevice::ReadOwner | QFileDevice::WriteOwner | QFileDevice::WriteUser)) {
        cinfo << "Log file created and open: " << QString::fromStdString(logFile.filesystemFileName());
    } else if (logFile.open(QIODeviceBase::Append)) {
        cinfo << "Log file open" << QString::fromStdString(logFile.filesystemFileName());
    } else {
        cerror << "Log file openning error" << QString::fromStdString(logFile.filesystemFileName());
    }
}

void Logger::loggerHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg) {
    auto stdString = msg.toStdString();

    bool isNumber = true;
    int idx = 0;
    while (idx < stdString.size()) {
        if (!(stdString[idx] >= '0' && stdString[idx] <= '9')) {
            isNumber = false;
            break;
        }
        idx++;
    }

    if (isNumber && type == QtDebugMsg) {
        return;
    }

    bool isFromQML = QString(context.category) == QString("qml");
    QDateTime currentDateTime = QDateTime::currentDateTime();
    QString formattedDateTime = currentDateTime.toString("yyyy-MM-dd hh:mm:ss.zzz ");

    QString fileLine;
    switch (type) {
        case QtDebugMsg:
            if (LOG_LEVEL >= 4) {
                if (isFromQML) {
                    std::clog << "\033[1;34m" << "Debug:\t\t" << stdString << "\033[0m" << std::endl;
                } else {
                    std::clog << "\033[1;34m" << "Debug:\t\t" << "[" << context.category << "] " << stdString
                              << "\033[0m" << std::endl;
                }
            }
            fileLine =
                formattedDateTime +
                (!isFromQML ? ("[" + QString(context.category) + "] " + "Debug: " + msg)
                            : ("Debug: " + msg + " (" + context.file + ":" + QString::number(context.line) + ")")) +
                "\n";
            break;
        case QtInfoMsg:
            if (LOG_LEVEL >= 3) {
                if (isFromQML) {
                    std::clog << "\033[1;32m" << "Info:\t\t" << stdString << "\033[0m" << std::endl;
                } else {
                    std::clog << "\033[1;32m" << "Info:\t\t" << "[" << context.category << "] " << stdString
                              << "\033[0m" << std::endl;
                }
            }
            fileLine =
                formattedDateTime +
                (!isFromQML ? ("[" + QString(context.category) + "] " + "Info: " + msg)
                            : ("Info: " + msg + " (" + context.file + ":" + QString::number(context.line) + ")")) +
                "\n";
            break;
        case QtWarningMsg:
            if (LOG_LEVEL >= 2) {
                if (isFromQML) {
                    std::clog << "\033[1;33m" << "Warning:\t" << stdString << "\033[0m" << std::endl;
                } else {
                    std::clog << "\033[1;33m" << "Warning:\t" << "[" << context.category << "] " << stdString
                              << "\033[0m" << std::endl;
                }
            }
            fileLine =
                formattedDateTime +
                (!isFromQML ? ("[" + QString(context.category) + "] " + "Warning: " + msg)
                            : ("Warning: " + msg + " (" + context.file + ":" + QString::number(context.line) + ")")) +
                "\n";
            break;
        case QtCriticalMsg:
            if (LOG_LEVEL >= 1) {
                if (isFromQML) {
                    std::clog << "\033[1;31m" << "Critical:\t" << stdString << "\033[0m" << std::endl;
                } else {
                    std::clog << "\033[1;31m" << "Critical:\t" << "[" << context.category << "] " << stdString
                              << "\033[0m" << std::endl;
                }
            }
            fileLine =
                formattedDateTime +
                (!isFromQML ? ("[" + QString(context.category) + "] " + "Critical: " + msg)
                            : ("Critical: " + msg + " (" + context.file + ":" + QString::number(context.line) + ")")) +
                "\n";
            break;
        case QtFatalMsg:
            if (LOG_LEVEL >= 0) {
                if (isFromQML) {
                    std::clog << "\033[1;35m" << "Fatal:\t\t" << stdString << "\033[0m" << std::endl;
                } else {
                    std::clog << "\033[1;35m" << "Fatal:\t\t" << "[" << context.category << "] " << stdString
                              << "\033[0m" << std::endl;
                }
            }
            fileLine =
                formattedDateTime +
                (!isFromQML ? ("[" + QString(context.category) + "] " + "Fatal: " + msg)
                            : ("Fatal: " + msg + " (" + context.file + ":" + QString::number(context.line) + ")")) +
                "\n";
            if (logFile.isOpen()) {
                logFile.write(fileLine.toLocal8Bit());
            }
            abort();
    }
    if (logFile.isOpen()) {
        logFile.write(fileLine.toLocal8Bit());
    }

    if (logFile.size() > LOG_ROTATION) {
        logFile.close();

        // rename old files
        if (logsDir.exists("autoapp" + QString::number(LOG_RETENTION - 1) + ".log")) {
            logsDir.remove("autoapp" + QString::number(LOG_RETENTION - 1) + ".log");
        }
        for (int i = (LOG_RETENTION - 1); i > 0; i--) {
            logsDir.rename("autoapp" + QString::number(i) + ".log", "autoapp" + QString::number(i + 1) + ".log");
        }
        logsDir.rename("autoapp.log", "autoapp1.log");

        // create new file
        logFile.open(QIODeviceBase::WriteOnly);
    }
}
