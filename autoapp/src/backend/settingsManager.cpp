#include "settingsManager.h"

#include <QGuiApplication>

SettingsManager::SettingsManager() : category("SETTINGS MANAGER") {
    setDefault();
}

int SettingsManager::getLoggerRotation() {
    return value(LOGGER_ROTATION_KEY).toInt();
}

void SettingsManager::setLoggerRotation(int value) {
    setValue(LOGGER_ROTATION_KEY, value);
}

int SettingsManager::getLoggerRetention() {
    return value(LOGGER_RETENTION_KEY).toInt();
}

void SettingsManager::setLoggerRetention(int value) {
    setValue(LOGGER_RETENTION_KEY, value);
}

QString SettingsManager::getLoggerPath() {
    return value(LOGGER_PATH_KEY).toString();
}

void SettingsManager::setLoggerPath(QString value) {
    setValue(LOGGER_PATH_KEY, value);
}

int SettingsManager::getLoggetStdOutputLevel() {
    return value(LOGGER_STD_OUTPUT_LEVEL_KEY).toInt();
}

void SettingsManager::setLoggetStdOutputLevel(int value) {
    setValue(LOGGER_STD_OUTPUT_LEVEL_KEY, value);
}

void SettingsManager::setDefault(bool force) {
    setDefaultValue(force, LOGGER_ROTATION_KEY, LOGGER_ROTATION_VALUE);
    setDefaultValue(force, LOGGER_RETENTION_KEY, LOGGER_RETENTION_VALUE);
    setDefaultValue(force, LOGGER_PATH_KEY, LOGGER_PATH_VALUE);
    setDefaultValue(force, LOGGER_STD_OUTPUT_LEVEL_KEY, LOGGER_STD_OUTPUT_LEVEL_VALUE);
}

void SettingsManager::setDefaultValue(bool force, QString key, QVariant value) {
    if (force || !this->value(key).isValid())
        setValue(key, value);
}

void SettingsManager::defineSettings(QString appName) {
    QGuiApplication::setOrganizationName(company);
    QGuiApplication::setOrganizationDomain(companyDomain);
    QGuiApplication::setApplicationName(appName);
}
