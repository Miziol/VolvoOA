#ifndef AUTOAPP_SETTINGSMANAGER_H
#define AUTOAPP_SETTINGSMANAGER_H

#include <QBluetoothAddress>
#include <QSettings>

#include "f1x/openauto/autoapp/Configuration/IConfiguration.hpp"
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
    
    inline static const QString GENERAL_SHOW_CLOCK_KEY = "general/show_clock";
    inline static const bool GENERAL_SHOW_CLOCK_VALUE = true;
    inline static const QString GENERAL_LEFT_HAND_DRIVE_KEY = "general/left_hand_drive";
    inline static const bool GENERAL_LEFT_HAND_DRIVE_VALUE = true;

    inline static const QString VIDEO_FPS_KEY = "video/fps";
    inline static const f1x::aasdk::proto::enums::VideoFPS::Enum VIDEO_FPS_VALUE = f1x::aasdk::proto::enums::VideoFPS::_60;
    inline static const QString VIDEO_RESOLUTION_KEY = "video/resolution";
    inline static const f1x::aasdk::proto::enums::VideoResolution::Enum VIDEO_RESOLUTION_VALUE = f1x::aasdk::proto::enums::VideoResolution::_480p;
    inline static const QString VIDEO_SCREEN_DPI_KEY = "video/screen_dpi";
    inline static const int VIDEO_SCREEN_DPI_VALUE = 140;
    inline static const QString VIDEO_MARGIN_WIDTH_KEY = "video/margin_width";
    inline static const int VIDEO_MARGIN_WIDTH_VALUE = 0;
    inline static const QString VIDEO_MARGIN_HEIGHT_KEY = "video/margin_height";
    inline static const int VIDEO_MARGIN_HEIGHT_VALUE = 0;

    inline static const QString NAVIGATION_TOUCHSCREEN_ENABLED_KEY = "navigation/touchscreen_enabled";
    inline static const bool NAVIGATION_TOUCHSCREEN_ENABLED_VALUE = true;

    inline static const QString NAVIGATION_ENTER_KEY = "navigation/enter";
    inline static const bool NAVIGATION_ENTER_VALUE = true;
    inline static const QString NAVIGATION_ARROWS_KEY = "navigation/arrows";
    inline static const bool NAVIGATION_ARROWS_VALUE = true;
    inline static const QString NAVIGATION_SCROLL_WHEEL_KEY = "navigation/scroll_wheel";
    inline static const bool NAVIGATION_SCROLL_WHEEL_VALUE = true;
    inline static const QString NAVIGATION_BACK_KEY = "navigation/back";
    inline static const bool NAVIGATION_BACK_VALUE = true;
    inline static const QString NAVIGATION_HOME_KEY = "navigation/home";
    inline static const bool NAVIGATION_HOME_VALUE = true;
    inline static const QString NAVIGATION_MEDIA_KEY = "navigation/media";
    inline static const bool NAVIGATION_MEDIA_VALUE = true;
    inline static const QString NAVIGATION_VOICE_COMMAND_KEY = "navigation/voice_command";
    inline static const bool NAVIGATION_VOICE_COMMAND_VALUE = false;
    inline static const QString NAVIGATION_CHARS_KEY = "navigation/chars";
    inline static const bool NAVIGATION_CHARS_VALUE = false;
    inline static const QString NAVIGATION_LETTERS_KEY = "navigation/letters";
    inline static const bool NAVIGATION_LETTERS_VALUE = false;
    inline static const QString NAVIGATION_NUMBERS_KEY = "navigation/numbers";
    inline static const bool NAVIGATION_NUMBERS_VALUE = false;

    inline static const QString BLUETOOTH_ADAPTER_TYPE_KEY = "bluetooth/adapter_type"; // TODO remove ?
    inline static const f1x::openauto::autoapp::configuration::BluetoothAdapterType BLUETOOTH_ADAPTER_TYPE_VALUE = f1x::openauto::autoapp::configuration::BluetoothAdapterType::LOCAL;
    inline static const QString BLUETOOTH_REMOTE_ADAPTER_ADDRESS_KEY = "bluetooth/remote_adapter_address";
    inline static const QBluetoothAddress BLUETOOTH_REMOTE_ADAPTER_ADDRESS_VALUE = QBluetoothAddress();

private:
public slots:
    // Logger
    int getLoggerRotation();
    void setLoggerRotation(int value);
    int getLoggerRetention();
    void setLoggerRetention(int value);
    QString getLoggerPath();
    void setLoggerPath(QString value);
    int getLoggetStdOutputLevel();
    void setLoggetStdOutputLevel(int value);

    // Andoid Auto
    bool isLeftHandDrive();
    void setLeftHandDrive(bool value);
    bool getShowClock();
    void setShowClock(bool value);

    f1x::aasdk::proto::enums::VideoFPS::Enum getVideoFPS() const;
    void setVideoFPS(f1x::aasdk::proto::enums::VideoFPS::Enum value);
    f1x::aasdk::proto::enums::VideoResolution::Enum getVideoResolution() const;
    void setVideoResolution(f1x::aasdk::proto::enums::VideoResolution::Enum value);
    size_t getScreenDPI() const;
    void setScreenDPI(size_t value);
    QRect getVideoMargins() const; // TODO remove ?
    void setVideoMargins(QRect value);

    bool getTouchscreenEnabled() const;
    void setTouchscreenEnabled(bool value);
    QList<f1x::aasdk::proto::enums::ButtonCode::Enum> getButtonCodes() const;
    void setButtonCodes(const QList<f1x::aasdk::proto::enums::ButtonCode::Enum>& value);

    f1x::openauto::autoapp::configuration::BluetoothAdapterType getBluetoothAdapterType() const;
    void setBluetoothAdapterType(f1x::openauto::autoapp::configuration::BluetoothAdapterType value);
    QBluetoothAddress getBluetoothRemoteAdapterAddress() const;
    void setBluetoothRemoteAdapterAddress(const QBluetoothAddress value);

private:
    void setDefault(bool force = false);
    void setDefaultValue(bool force, QString key, QVariant value);

public:
    static void defineSettings(QString appName);
};

#endif  // AUTOAPP_SETTINGSMANAGER_H
