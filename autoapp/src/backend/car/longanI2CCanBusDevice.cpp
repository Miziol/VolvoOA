#include "longanI2CCanBusDevice.h"

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <QtConcurrent/QtConcurrent>
#include <iostream>

LonganI2CCanBusDevice::LonganI2CCanBusDevice(QObject *parent) : QCanBusDevice(parent) {}

LonganI2CCanBusDevice::~LonganI2CCanBusDevice() {
    disconnectDevice();
}

bool LonganI2CCanBusDevice::open() {
    setState(QCanBusDevice::ConnectedState);

    deviceFileDescriptor = ::open(("/dev/" + bus).toLocal8Bit().constData(), O_RDWR);
    if (deviceFileDescriptor < 0) {
        setError("Failed to open I2C device", QCanBusDevice::ConnectionError);
        setState(QCanBusDevice::UnconnectedState);
        return false;
    }

    if (ioctl(deviceFileDescriptor, I2C_SLAVE, address) < 0) {
        setError("Failed to set I2C address", QCanBusDevice::ConnectionError);
        setState(QCanBusDevice::UnconnectedState);
        ::close(deviceFileDescriptor);
        return false;
    }

    setState(QCanBusDevice::ConnectedState);

    // Start reader thread
    QtConcurrent::run([this]() { readLoop(); });
    return true;
}

void LonganI2CCanBusDevice::close() {
    setState(QCanBusDevice::ClosingState);
    if (deviceFileDescriptor >= 0) {
        ::close(deviceFileDescriptor);
        deviceFileDescriptor = -1;

        setState(QCanBusDevice::UnconnectedState);
    }
}

bool LonganI2CCanBusDevice::writeFrame(const QCanBusFrame &frame) {
    if (state() != ConnectedState)
        return false;

    QByteArray payload = frame.payload();
    uint8_t buffer[16] = {0};
    buffer[0] = (frame.frameId() >> 8) & 0xFF;
    buffer[1] = frame.frameId() & 0xFF;
    buffer[2] = payload.size();
    memcpy(&buffer[3], payload.constData(), payload.size());

    ::write(deviceFileDescriptor, buffer, 3 + payload.size());
    return true;
}

QString LonganI2CCanBusDevice::interpretErrorFrame(const QCanBusFrame &frame) {
    return QString();  // TODO
}

void LonganI2CCanBusDevice::readLoop() {
    while (state() == ConnectedState) {
        uint8_t buffer[16];
        int n = ::read(deviceFileDescriptor, buffer, sizeof(buffer));
        if (n > 3) {
            int id = (buffer[0] << 8) | buffer[1];
            int len = buffer[2];
            QByteArray data((char *)&buffer[3], len);
            QCanBusFrame frame(id, data);
            enqueueReceivedFrames({frame});
        }
    }
}