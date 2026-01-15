#include "longanI2CCanBusDevice.h"

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <QtConcurrent/QtConcurrent>
#include <iostream>

#include "../../external/LonganLabs I2C CAN/I2C_CAN_dfs.h"

LonganI2CCanBusDevice::LonganI2CCanBusDevice(QObject *parent)
    : QCanBusDevice(parent), bus("i2c-1"), address(DEFAULT_I2C_ADDR), deviceFileDescriptor(-1) {}

LonganI2CCanBusDevice::~LonganI2CCanBusDevice() {
    disconnectDevice();
}

bool LonganI2CCanBusDevice::open() {
    setState(QCanBusDevice::ConnectedState);

    deviceFileDescriptor = ::open(("/dev/" + bus).toLocal8Bit().constData(), O_RDWR);
    if (deviceFileDescriptor < 0) {
        setError("Failed to open I2C device " + QString::fromLocal8Bit(strerror(errno)),
                 QCanBusDevice::ConnectionError);
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

void LonganI2CCanBusDevice::setConfigurationParameter(ConfigurationKey key, const QVariant &value) {
    switch (key) {
        case ConfigurationKey::RawFilterKey:
            break;
        case ConfigurationKey::ErrorFilterKey:
            break;
        case ConfigurationKey::LoopbackKey:
            break;
        case ConfigurationKey::ReceiveOwnKey:
            break;
        case ConfigurationKey::BitRateKey:
            setSpeed(value);
            break;
        case ConfigurationKey::CanFdKey:
            break;
        case ConfigurationKey::DataBitRateKey:
            break;
        case ConfigurationKey::ProtocolKey:
            break;
        case ConfigurationKey::UserKey:
            break;
        default:
            break;
    }
}

bool LonganI2CCanBusDevice::writeFrame(const QCanBusFrame &frame) {
    if (state() != ConnectedState)
        return false;

    QByteArray data;
    for (int i = 3; i >= 0; i--) {
        data.append(0xFF & (frame.frameId() >> (i * 8)));
    }
    data.append(frame.hasExtendedFrameFormat());
    data.append(frame.frameType() == QCanBusFrame::RemoteRequestFrame);
    data.append(frame.payload().size());
    data.append(frame.payload());
    data.append(makeCheckSum(data));
    return setRegister(REG_SEND, data);
}

QString LonganI2CCanBusDevice::interpretErrorFrame(const QCanBusFrame &frame) {
    return QString(); // TODO
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

bool LonganI2CCanBusDevice::setRegister(unsigned char reg, QByteArray data) {
    data.prepend(reg);
    int transferedBytes = ::write(deviceFileDescriptor, data.constData(), data.size());
    return transferedBytes == data.size();
}

void LonganI2CCanBusDevice::setSpeed(const QVariant &speed) {
    setRegister(REG_BAUD, QByteArray().append(mapSpeedToDefine(speed)));
}

int LonganI2CCanBusDevice::mapSpeedToDefine(const QVariant &speed) {
    switch (speed.toInt()) {
        case 5000:
            return CAN_5KBPS;
        case 10000:
            return CAN_10KBPS;
        case 20000:
            return CAN_20KBPS;
        case 25000:
            return CAN_25KBPS;
        case 31200: // from docs
        case 31250: // from #define name
            return CAN_31K25BPS;
        case 33000:
            return CAN_33KBPS;
        case 40000:
            return CAN_40KBPS;
        case 50000:
            return CAN_50KBPS;
        case 80000:
            return CAN_80KBPS;
        case 83300:
            return CAN_83K3BPS;
        case 95000:
            return CAN_95KBPS;
        case 100000:
            return CAN_100KBPS;
        case 125000:
            return CAN_125KBPS;
        case 200000:
            return CAN_200KBPS;
        case 250000:
            return CAN_250KBPS;
        case 500000:
            return CAN_500KBPS;
        case 666000:
            return CAN_666KBPS;
        case 1000000:
            return CAN_1000KBPS;
        default:
            return 0;
    }
}

unsigned char LonganI2CCanBusDevice::makeCheckSum(QByteArray data) {
    unsigned long sum = 0;
    for (const auto &byte : data) {
        sum += byte;
    }

    if (sum > 0xFF) {
        sum = ~sum;
        sum += 1;
    }

    return sum & 0xFF;
}
