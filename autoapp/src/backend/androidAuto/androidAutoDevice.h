#ifndef AUTOAPP_ANDROIDAUTODEVICE_H
#define AUTOAPP_ANDROIDAUTODEVICE_H

#include <libusb.h>
#include <openssl/ssl.h>

#include <QAudioFormat>
#include <QObject>
#include <QTimer>

#include "../logging/loggingCategory.h"

const uint16_t VERSION_MAJOR = 1, VERSION_MINOR =1;

enum ChannelID
{
    CONTROL,
    INPUT,
    SENSOR,
    VIDEO,
    MEDIA_AUDIO,
    SPEECH_AUDIO,
    SYSTEM_AUDIO,
    AV_INPUT,
    BLUETOOTH,
    NONE = 255
};

enum EncryptionType {
    Plain = 0,
    Encrypted = 1 << 3,
  };

enum FrameType {
    First = 1,
    Last = 2,
    Bulk = First | Last,
  };

enum MessageTypeFlags {
    Control = 0,
    Specific = 1 << 2,
  };

enum MessageType {
    VersionRequest = 1,
    VersionResponse = 2,
    SslHandshake = 3,
    AuthComplete = 4,
    ServiceDiscoveryRequest = 5,
    ServiceDiscoveryResponse = 6,
    ChannelOpenRequest = 7,
    ChannelOpenResponse = 8,
    PingRequest = 0xb,
    PingResponse = 0xc,
    NavigationFocusRequest = 0x0d,
    NavigationFocusResponse = 0x0e,
    VoiceSessionRequest = 0x11,
    AudioFocusRequest = 0x12,
    AudioFocusResponse = 0x13,
  };

enum MediaMessageType {
    MediaWithTimestampIndication = 0x0000,
    MediaIndication = 0x0001,
    SetupRequest = 0x8000,
    StartIndication = 0x8001,
    SetupResponse = 0x8003,
    MediaAckIndication = 0x8004,
    VideoFocusIndication = 0x8008,
  };

enum InputChannelMessageType {
    None = 0,
    Event = 0x8001,
    HandshakeRequest = 0x8002,
    HandshakeResponse = 0x8003,
  };

class AndroidAutoDevice : public QObject {
    Q_OBJECT

    Q_PROPERTY(QString name READ getName CONSTANT)

public:
    AndroidAutoDevice(libusb_device *new_device, libusb_device_descriptor new_descriptor, SSL_CTX *ssl_ctx);
    ~AndroidAutoDevice();

private:
    QLoggingCategory category;

    libusb_device *device;
    libusb_device_handle *handle;
    libusb_device_descriptor descriptor;
    libusb_config_descriptor *config_descriptor;
    const libusb_interface_descriptor *interface;
    uint8_t inputAddress, outputAddress;

    SSL_CTX *context;
    SSL *sslConnection;
    BIO *inputBIO, *outputBIO;
    SSL_SESSION *sslSession;

    QTimer eventLoopTimer;
    int maxBufferSize;
    unsigned char *buffer = nullptr;
    libusb_transfer *transfer = nullptr;

    QAudioFormat audioFormat;

public slots:
    void open();
    void close();
    void start();
    void stop();

    QString getName();

    void sendMessageToChannel(ChannelID channel, uint8_t flags, std::vector<uint8_t> data);
    // void sendEncryptedMessageToChannel(ChannelID channel);
    void sendControlMessage(int flags, std::vector<uint8_t> data);

private:
    void extractEndpointAddresses();
    void configSSL();
    void clearSSL();
    void startListening(ChannelID channelID, void callback(libusb_transfer *));

    void static interfaceCallback(libusb_transfer *transfer);
};

#endif  // AUTOAPP_ANDROIDAUTODEVICE_H
