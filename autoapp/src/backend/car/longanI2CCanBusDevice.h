#ifndef ANDROID_AUTO_LONGANI2CCANBUSDEVICE_H
#define ANDROID_AUTO_LONGANI2CCANBUSDEVICE_H

#include <QtSerialBus/QCanBusDevice>

class LonganI2CCanBusDevice : public QCanBusDevice {
    Q_OBJECT
signals:

public:
    LonganI2CCanBusDevice(QObject *parent = nullptr);
    ~LonganI2CCanBusDevice() override;

private:
    QString bus;
    uint8_t address;
    int deviceFileDescriptor;

protected:
    bool open() override;
    void close() override;
    void setConfigurationParameter(ConfigurationKey key, const QVariant & value) override;

    bool writeFrame(const QCanBusFrame &frame) override;
    QString interpretErrorFrame(const QCanBusFrame &errorFrame) override;

private:
    void readLoop();

    bool setRegister(unsigned char reg, QByteArray data);
    void setSpeed(const QVariant &speed);

    int mapSpeedToDefine(const QVariant &speed);
    unsigned char makeCheckSum(QByteArray frame);

};

#endif  // ANDROID_AUTO_LONGANI2CCANBUSDEVICE_H
