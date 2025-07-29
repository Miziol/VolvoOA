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
}

bool SettingsManager::getShowClock() {
    return value(GENERAL_SHOW_CLOCK_KEY).toBool();
}

void SettingsManager::setShowClock(bool value) {
    setValue(GENERAL_SHOW_CLOCK_KEY, value);
}

f1x::aasdk::proto::enums::VideoFPS::Enum SettingsManager::getVideoFPS() const {
    return static_cast<f1x::aasdk::proto::enums::VideoFPS::Enum>(value(VIDEO_FPS_KEY).toInt());
}

void SettingsManager::setVideoFPS(f1x::aasdk::proto::enums::VideoFPS::Enum value) {
    setValue(VIDEO_FPS_KEY, value);
}

f1x::aasdk::proto::enums::VideoResolution::Enum SettingsManager::getVideoResolution() const {
    return static_cast<f1x::aasdk::proto::enums::VideoResolution_Enum>(value(VIDEO_RESOLUTION_KEY).toInt());
}

void SettingsManager::setVideoResolution(f1x::aasdk::proto::enums::VideoResolution::Enum value) {
    setValue(VIDEO_RESOLUTION_KEY, value);
}

size_t SettingsManager::getScreenDPI() const {
    return value(VIDEO_SCREEN_DPI_KEY).toInt();
}

void SettingsManager::setScreenDPI(size_t value) {
    setValue(VIDEO_SCREEN_DPI_KEY, int(value));
}

QRect SettingsManager::getVideoMargins() const {
    return QRect(0, 0, value(VIDEO_MARGIN_WIDTH_KEY).toInt(), value(VIDEO_MARGIN_HEIGHT_KEY).toInt());
}

void SettingsManager::setVideoMargins(QRect value) {
    setValue(VIDEO_MARGIN_WIDTH_KEY, value.width());
    setValue(VIDEO_MARGIN_HEIGHT_KEY, value.height());
}

bool SettingsManager::getTouchscreenEnabled() const {
    return value(NAVIGATION_TOUCHSCREEN_ENABLED_KEY).toBool();
}

void SettingsManager::setTouchscreenEnabled(bool value) {
    setValue(NAVIGATION_TOUCHSCREEN_ENABLED_KEY, value);
}

QList<f1x::aasdk::proto::enums::ButtonCode::Enum> SettingsManager::getButtonCodes() const {
    return QList<f1x::aasdk::proto::enums::ButtonCode::Enum>();  // TODO
}

void SettingsManager::setButtonCodes(const QList<f1x::aasdk::proto::enums::ButtonCode::Enum> &value) {
    // TODO
}

f1x::openauto::autoapp::configuration::BluetoothAdapterType SettingsManager::getBluetoothAdapterType() const {
    return static_cast<f1x::openauto::autoapp::configuration::BluetoothAdapterType>(
        value(BLUETOOTH_ADAPTER_TYPE_KEY).toInt());
}

void SettingsManager::setBluetoothAdapterType(f1x::openauto::autoapp::configuration::BluetoothAdapterType value) {
    setValue(BLUETOOTH_ADAPTER_TYPE_KEY, (int)value);
}

QBluetoothAddress SettingsManager::getBluetoothRemoteAdapterAddress() const {
    return QBluetoothAddress(value(BLUETOOTH_REMOTE_ADAPTER_ADDRESS_KEY).toString());
}

void SettingsManager::setBluetoothRemoteAdapterAddress(const QBluetoothAddress value) {
    setValue(BLUETOOTH_REMOTE_ADAPTER_ADDRESS_KEY, value.toString());
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

    setDefaultValue(force, NAVIGATION_TOUCHSCREEN_ENABLED_KEY, NAVIGATION_TOUCHSCREEN_ENABLED_VALUE);
    setDefaultValue(force, NAVIGATION_ENTER_KEY, NAVIGATION_ENTER_VALUE);
    setDefaultValue(force, NAVIGATION_ARROWS_KEY, NAVIGATION_ARROWS_VALUE);
    setDefaultValue(force, NAVIGATION_SCROLL_WHEEL_KEY, NAVIGATION_SCROLL_WHEEL_VALUE);
    setDefaultValue(force, NAVIGATION_BACK_KEY, NAVIGATION_BACK_VALUE);
    setDefaultValue(force, NAVIGATION_HOME_KEY, NAVIGATION_HOME_VALUE);
    setDefaultValue(force, NAVIGATION_MEDIA_KEY, NAVIGATION_MEDIA_VALUE);
    setDefaultValue(force, NAVIGATION_VOICE_COMMAND_KEY, NAVIGATION_VOICE_COMMAND_VALUE);
    setDefaultValue(force, NAVIGATION_CHARS_KEY, NAVIGATION_CHARS_VALUE);
    setDefaultValue(force, NAVIGATION_LETTERS_KEY, NAVIGATION_LETTERS_VALUE);
    setDefaultValue(force, NAVIGATION_NUMBERS_KEY, NAVIGATION_NUMBERS_VALUE);

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
