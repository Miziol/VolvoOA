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

bool SettingsManager::isLeftHandDrive() {
    return value(GENERAL_LEFT_HAND_DRIVE_KEY).toBool();
}

void SettingsManager::setLeftHandDrive(bool value) {
    setValue(GENERAL_LEFT_HAND_DRIVE_KEY, value);
    emit leftHandDriveChanged();
}

bool SettingsManager::getShowClock() {
    return value(GENERAL_SHOW_CLOCK_KEY).toBool();
}

void SettingsManager::setShowClock(bool value) {
    setValue(GENERAL_SHOW_CLOCK_KEY, value);
    emit showClockChanged();
}

int SettingsManager::getVideoFPSInt() {
    return value(VIDEO_FPS_KEY).toInt();
}

f1x::aasdk::proto::enums::VideoFPS::Enum SettingsManager::getVideoFPS() const {
    return static_cast<f1x::aasdk::proto::enums::VideoFPS::Enum>(value(VIDEO_FPS_KEY).toInt());
}

void SettingsManager::setVideoFPSInt(int value) {
    setValue(VIDEO_FPS_KEY, value);
    emit videoFPSChanged();
}

void SettingsManager::setVideoFPS(f1x::aasdk::proto::enums::VideoFPS::Enum value) {
    setValue(VIDEO_FPS_KEY, value);
    emit videoFPSChanged();
}

int SettingsManager::getVideoResolutionInt() {
    return value(VIDEO_RESOLUTION_KEY).toInt();
}

f1x::aasdk::proto::enums::VideoResolution::Enum SettingsManager::getVideoResolution() const {
    return static_cast<f1x::aasdk::proto::enums::VideoResolution_Enum>(value(VIDEO_RESOLUTION_KEY).toInt());
}

void SettingsManager::setVideoResolutionInt(int value) {
    setValue(VIDEO_RESOLUTION_KEY, value);
    emit videoResolutionChanged();
}

void SettingsManager::setVideoResolution(f1x::aasdk::proto::enums::VideoResolution::Enum value) {
    setValue(VIDEO_RESOLUTION_KEY, value);
    emit videoResolutionChanged();
}

size_t SettingsManager::getScreenDPI() const {
    return value(VIDEO_SCREEN_DPI_KEY).toInt();
}

void SettingsManager::setScreenDPI(size_t value) {
    setValue(VIDEO_SCREEN_DPI_KEY, int(value));
    emit screenDPIChanged();
}

QRect SettingsManager::getVideoMargins() const {
    return QRect(0, 0, value(VIDEO_MARGIN_WIDTH_KEY).toInt(), value(VIDEO_MARGIN_HEIGHT_KEY).toInt());
}

void SettingsManager::setVideoMargins(QRect value) {
    setValue(VIDEO_MARGIN_WIDTH_KEY, value.width());
    setValue(VIDEO_MARGIN_HEIGHT_KEY, value.height());
    emit videoMarginsChanged();
}

bool SettingsManager::getShowCursor() const {
    return value(NAVIGATION_SHOW_CURSOR_KEY).toBool();
}

void SettingsManager::setShowCursor(bool value) {
    setValue(NAVIGATION_SHOW_CURSOR_KEY, value);
    emit showCursorChanged();
}

f1x::openauto::autoapp::configuration::BluetoothAdapterType SettingsManager::getBluetoothAdapterType() const {
    return static_cast<f1x::openauto::autoapp::configuration::BluetoothAdapterType>(
        value(BLUETOOTH_ADAPTER_TYPE_KEY).toInt());
}

void SettingsManager::setBluetoothAdapterType(f1x::openauto::autoapp::configuration::BluetoothAdapterType value) {
    setValue(BLUETOOTH_ADAPTER_TYPE_KEY, (int)value);
    emit bluetoothAdapterTypeChanged();
}

QBluetoothAddress SettingsManager::getBluetoothRemoteAdapterAddress() const {
    return QBluetoothAddress(value(BLUETOOTH_REMOTE_ADAPTER_ADDRESS_KEY).toString());
}

void SettingsManager::setBluetoothRemoteAdapterAddress(const QBluetoothAddress value) {
    setValue(BLUETOOTH_REMOTE_ADAPTER_ADDRESS_KEY, value.toString());
    emit bluetoothAddressChanged();
}

void SettingsManager::setDefault(bool force) {
    setDefaultValue(force, LOGGER_ROTATION_KEY, LOGGER_ROTATION_VALUE);
    setDefaultValue(force, LOGGER_RETENTION_KEY, LOGGER_RETENTION_VALUE);
    setDefaultValue(force, LOGGER_PATH_KEY, LOGGER_PATH_VALUE);
    setDefaultValue(force, LOGGER_STD_OUTPUT_LEVEL_KEY, LOGGER_STD_OUTPUT_LEVEL_VALUE);

    setDefaultValue(force, GENERAL_SHOW_CLOCK_KEY, GENERAL_SHOW_CLOCK_VALUE);
    setDefaultValue(force, GENERAL_LEFT_HAND_DRIVE_KEY, GENERAL_LEFT_HAND_DRIVE_VALUE);

    setDefaultValue(force, VIDEO_FPS_KEY, VIDEO_FPS_VALUE);
    setDefaultValue(force, VIDEO_RESOLUTION_KEY, VIDEO_RESOLUTION_VALUE);
    setDefaultValue(force, VIDEO_SCREEN_DPI_KEY, VIDEO_SCREEN_DPI_VALUE);
    setDefaultValue(force, VIDEO_MARGIN_WIDTH_KEY, VIDEO_MARGIN_WIDTH_VALUE);
    setDefaultValue(force, VIDEO_MARGIN_HEIGHT_KEY, VIDEO_MARGIN_HEIGHT_VALUE);

    setDefaultValue(force, NAVIGATION_SHOW_CURSOR_KEY, NAVIGATION_SHOW_CURSOR_VALUE);

    setDefaultValue(force, BLUETOOTH_ADAPTER_TYPE_KEY, (int)(BLUETOOTH_ADAPTER_TYPE_VALUE));
    setDefaultValue(force, BLUETOOTH_REMOTE_ADAPTER_ADDRESS_KEY, BLUETOOTH_REMOTE_ADAPTER_ADDRESS_VALUE.toString());
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
