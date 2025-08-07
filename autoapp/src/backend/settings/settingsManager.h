#ifndef AUTOAPP_SETTINGSMANAGER_H
#define AUTOAPP_SETTINGSMANAGER_H

#include <aasdk_proto/ButtonCodeEnum.pb.h>
#include <aasdk_proto/VideoFPSEnum.pb.h>
#include <aasdk_proto/VideoResolutionEnum.pb.h>

#include <QBluetoothAddress>
#include <QRect>
#include <QSettings>

#include "../logging/loggingCategory.h"
#include "f1x/openauto/autoapp/Configuration/BluetootAdapterType.hpp"

const QString company = "Miziol";
const QString companyDomain = "github.com/Miziol";

class SettingsManager : public QSettings {
    Q_OBJECT

signals:
    void showClockChanged();
    void leftHandDriveChanged();

    void videoFPSChanged();
    void videoResolutionChanged();
    void screenDPIChanged();
    void videoMarginsChanged();

    void showCursorChanged();

    void bluetoothAdapterTypeChanged();
    void bluetoothAddressChanged();

    void appLanguageChanged(QString lang);  // TODO impl translations

public:
    Q_PROPERTY(bool showClock READ getShowClock WRITE setShowClock NOTIFY showClockChanged)
    Q_PROPERTY(bool leftHandDrive READ isLeftHandDrive WRITE setLeftHandDrive NOTIFY leftHandDriveChanged)

    Q_PROPERTY(int videoFPS READ getVideoFPSInt WRITE setVideoFPSInt NOTIFY videoFPSChanged) // TODO enum
    Q_PROPERTY(int videoResolution READ getVideoResolutionInt WRITE setVideoResolutionInt NOTIFY videoResolutionChanged) // TODO enum
    Q_PROPERTY(int screenDPI READ getScreenDPI WRITE setScreenDPI NOTIFY screenDPIChanged)
    // Q_PROPERTY() // TODO video margin

    Q_PROPERTY(bool showCursor READ getShowCursor WRITE setShowCursor NOTIFY showCursorChanged)

public:
    SettingsManager();

private:
    QLoggingCategory category;

    inline static const QString LOGGER_ROTATION_KEY = "logger/rotation";
    inline static const int LOGGER_ROTATION_VALUE = 100'000'000;
    inline static const QString LOGGER_RETENTION_KEY = "logger/retention";
    inline static const int LOGGER_RETENTION_VALUE = 10;
    inline static const QString LOGGER_PATH_KEY = "logger/path";
    inline static const QString LOGGER_PATH_VALUE = "/var/log/";
    inline static const QString LOGGER_STD_OUTPUT_LEVEL_KEY = "logger/output_level";
    inline static const int LOGGER_STD_OUTPUT_LEVEL_VALUE = 3;

    inline static const QString GENERAL_SHOW_CLOCK_KEY = "system/show_clock";
    inline static const bool GENERAL_SHOW_CLOCK_VALUE = true;
    inline static const QString GENERAL_LEFT_HAND_DRIVE_KEY = "system/left_hand_drive";
    inline static const bool GENERAL_LEFT_HAND_DRIVE_VALUE = true;

    inline static const QString VIDEO_FPS_KEY = "video/fps";
    inline static const f1x::aasdk::proto::enums::VideoFPS::Enum VIDEO_FPS_VALUE =
        f1x::aasdk::proto::enums::VideoFPS::_60;
    inline static const QString VIDEO_RESOLUTION_KEY = "video/resolution";
    inline static const f1x::aasdk::proto::enums::VideoResolution::Enum VIDEO_RESOLUTION_VALUE =
        f1x::aasdk::proto::enums::VideoResolution::_480p;
    inline static const QString VIDEO_SCREEN_DPI_KEY = "video/screen_dpi";
    inline static const int VIDEO_SCREEN_DPI_VALUE = 140;
    inline static const QString VIDEO_MARGIN_WIDTH_KEY = "video/margin_width";
    inline static const int VIDEO_MARGIN_WIDTH_VALUE = 0;
    inline static const QString VIDEO_MARGIN_HEIGHT_KEY = "video/margin_height";
    inline static const int VIDEO_MARGIN_HEIGHT_VALUE = 0;

    inline static const QString NAVIGATION_SHOW_CURSOR_KEY = "navigation/show_cursor";
    inline static const bool NAVIGATION_SHOW_CURSOR_VALUE = false;

    // TODO remove ?
    inline static const QString BLUETOOTH_ADAPTER_TYPE_KEY = "bluetooth/adapter_type";
    inline static const f1x::openauto::autoapp::configuration::BluetoothAdapterType BLUETOOTH_ADAPTER_TYPE_VALUE =
        f1x::openauto::autoapp::configuration::BluetoothAdapterType::LOCAL;
    inline static const QString BLUETOOTH_REMOTE_ADAPTER_ADDRESS_KEY = "bluetooth/remote_adapter_address";
    inline static const QBluetoothAddress BLUETOOTH_REMOTE_ADAPTER_ADDRESS_VALUE = QBluetoothAddress();
    // END TODO

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

    int getVideoFPSInt(); // TODO remove
    f1x::aasdk::proto::enums::VideoFPS::Enum getVideoFPS() const;
    void setVideoFPSInt(int value); // TODO remove
    void setVideoFPS(f1x::aasdk::proto::enums::VideoFPS::Enum value);
    int getVideoResolutionInt(); // TODO remove
    f1x::aasdk::proto::enums::VideoResolution::Enum getVideoResolution() const;
    void setVideoResolutionInt(int value); // TODO remove
    void setVideoResolution(f1x::aasdk::proto::enums::VideoResolution::Enum value);
    size_t getScreenDPI() const;
    void setScreenDPI(size_t value);
    QRect getVideoMargins() const;
    void setVideoMargins(QRect value);

    bool getShowCursor() const;
    void setShowCursor(bool value);

    // TODO remove ?
    f1x::openauto::autoapp::configuration::BluetoothAdapterType getBluetoothAdapterType() const;
    void setBluetoothAdapterType(f1x::openauto::autoapp::configuration::BluetoothAdapterType value);
    QBluetoothAddress getBluetoothRemoteAdapterAddress() const;
    void setBluetoothRemoteAdapterAddress(const QBluetoothAddress value);
    // END TODO

private:
    void setDefault(bool force = false);
    void setDefaultValue(bool force, QString key, QVariant value);

public:
    static void defineSettings(QString appName);
};

#endif  // AUTOAPP_SETTINGSMANAGER_H
