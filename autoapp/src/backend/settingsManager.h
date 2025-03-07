#ifndef AUTOAPP_SETTINGSMANAGER_H
#define AUTOAPP_SETTINGSMANAGER_H

#include <QSettings>

#include "logging/loggingCategory.h"

const QString company = "Miziol";
const QString companyDomain = "github.com/Miziol";

class SettingsManager : public QSettings {
    Q_OBJECT

signals:
    void appLanguageChanged(QString lang);

public:
    SettingsManager();

private:
    QLoggingCategory category;

public:
    inline static const QString LOGGER_ROTATION_KEY = "logger/rotation";
    inline static const int LOGGER_ROTATION_VALUE = 100'000'000;
    inline static const QString LOGGER_RETENTION_KEY = "logger/retention";
    inline static const int LOGGER_RETENTION_VALUE = 10;
    inline static const QString LOGGER_PATH_KEY = "logger/path";
    inline static const QString LOGGER_PATH_VALUE = "/var/log/";
    inline static const QString LOGGER_STD_OUTPUT_LEVEL_KEY = "logger/output_level";
    inline static const int LOGGER_STD_OUTPUT_LEVEL_VALUE = 3;

private:
public slots:
    int getLoggerRotation();
    void setLoggerRotation(int value);
    int getLoggerRetention();
    void setLoggerRetention(int value);
    QString getLoggerPath();
    void setLoggerPath(QString value);
    int getLoggetStdOutputLevel();
    void setLoggetStdOutputLevel(int value);

private:
    void setDefault(bool force = false);
    void setDefaultValue(bool force, QString key, QVariant value);

public:
    static void defineSettings(QString appName);
};

#endif  // AUTOAPP_SETTINGSMANAGER_H
