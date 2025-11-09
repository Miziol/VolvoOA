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
     // QLoggingCategory category; // TODO check if needed

    QString bus;
    uint8_t address;
    int deviceFileDescriptor;

protected:
    bool open() override;
    void close() override;

    bool writeFrame(const QCanBusFrame &frame) override;
    QString interpretErrorFrame(const QCanBusFrame &errorFrame) override;

private:
    void readLoop();
};

#endif  // ANDROID_AUTO_LONGANI2CCANBUSDEVICE_H
