#ifndef AUTOAPP_SETTINGSMANAGER_H
#define AUTOAPP_SETTINGSMANAGER_H

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
    inline static const QString VIDEO_OMX_LAYER_KEY = "video/omx_layer";
    inline static const int VIDEO_OMX_LAYER_VALUE = 1;
    inline static const QString VIDEO_MARGIN_WIDTH_KEY = "video/margin_width";
    inline static const QRect VIDEO_MARGIN_WIDTH_VALUE = QRect(0,0,0,0);
    inline static const QString VIDEO_MARGIN_HEIGHT_KEY = "video/margin_height";
    inline static const QRect VIDEO_MARGIN_HEIGHT_VALUE = QRect(0,0,0,0);

    inline static const QString cBluetoothAdapterTypeKey;
    inline static const QString cBluetoothRemoteAdapterAddressKey;

    inline static const QString cInputEnableTouchscreenKey;
    inline static const QString cInputPlayButtonKey;
    inline static const QString cInputPauseButtonKey;
    inline static const QString cInputTogglePlayButtonKey;
    inline static const QString cInputNextTrackButtonKey;
    inline static const QString cInputPreviousTrackButtonKey;
    inline static const QString cInputHomeButtonKey;
    inline static const QString cInputPhoneButtonKey;
    inline static const QString cInputCallEndButtonKey;
    inline static const QString cInputVoiceCommandButtonKey;
    inline static const QString cInputLeftButtonKey;
    inline static const QString cInputRightButtonKey;
    inline static const QString cInputUpButtonKey;
    inline static const QString cInputDownButtonKey;
    inline static const QString cInputScrollWheelButtonKey;
    inline static const QString cInputBackButtonKey;
    inline static const QString cInputEnterButtonKey;

    inline static const QString cInputArrowsButtonKey;
    inline static const QString cInputMediaButtonKey;
    inline static const QString cInputCharsButtonKey;
    inline static const QString cInputLettersButtonKey;
    inline static const QString cInputNumbersButtonKey;

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
    void setOMXLayerIndex(int32_t value);
    int32_t getOMXLayerIndex() const;
    void setVideoMargins(QRect value);
    QRect getVideoMargins() const;

    bool getTouchscreenEnabled() const;
    void setTouchscreenEnabled(bool value);
    QList<f1x::aasdk::proto::enums::ButtonCode::Enum> getButtonCodes() const;
    void setButtonCodes(const QList<f1x::aasdk::proto::enums::ButtonCode::Enum>& value);

    f1x::openauto::autoapp::configuration::BluetoothAdapterType getBluetoothAdapterType() const;
    void setBluetoothAdapterType(f1x::openauto::autoapp::configuration::BluetoothAdapterType value);
    std::string getBluetoothRemoteAdapterAddress() const;
    void setBluetoothRemoteAdapterAddress(const std::string& value);

private:
    void setDefault(bool force = false);
    void setDefaultValue(bool force, QString key, QVariant value);

public:
    static void defineSettings(QString appName);
};

#endif  // AUTOAPP_SETTINGSMANAGER_H
