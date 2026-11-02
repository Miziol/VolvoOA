#include "longanI2CCanBusDevice.h"

#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <sys/ioctl.h>
#include <unistd.h>

#include <QThread>
#include <QVariant>
#include <iostream>

#include "../../external/LonganLabs I2C CAN/I2C_CAN_dfs.h"

namespace {
constexpr int kReceiveBufferSize = 16;
constexpr int kMaxFrameLen = 8;

// ponytail: kalibracja sprzętowa — dobierz do głębokości bufora RX modułu i
// prędkości magistrali i2c. Przy 125 kbps worst case ~1 ms/ramkę, a moduł ma
// własny bufor RX (REG_DNUM), więc FIFO o głębokości D daje ~D ms zapasu.
// 1 ms jest bezpieczne dla D >= 2; przy 100 kHz i2c odczyt i tak trwa ~1 ms,
// więc realny interwał jest dłuższy. Przy szybszej magistrali skróć.
constexpr int kPollIntervalMs = 1;
}  // namespace

LonganI2CCanBusDevice::LonganI2CCanBusDevice(QObject *parent)
    : QCanBusDevice(parent),
      category("LONGAN I2C CAN"),
      bus("i2c-1"),
      address(DEFAULT_I2C_ADDR),
      deviceFileDescriptor(-1) {}

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
        deviceFileDescriptor = -1;
        return false;
    }

    setState(QCanBusDevice::ConnectedState);

    // Apply the configured bit rate before the worker starts (it owns the fd).
    const QVariant bitRate = configurationParameter(QCanBusDevice::BitRateKey);
    if (bitRate.isValid()) {
        setRegister(REG_BAUD, QByteArray().append(mapSpeedToDefine(bitRate)));
    }

    // Dedicated worker thread; Qt owns its lifecycle (no manual stop flag).
    // No context object on purpose: the functor then runs directly in the
    // worker thread (with a context it would be queued back onto the GUI thread).
    workerThread = new QThread(this);
    connect(workerThread, &QThread::started, [this]() { readLoop(); });
    workerThread->start();
    return true;
}

void LonganI2CCanBusDevice::close() {
    setState(QCanBusDevice::ClosingState);

    // The worker thread is the sole owner of the fd: stop it before closing.
    if (workerThread) {
        workerThread->requestInterruption();
        workerThread->wait();
        delete workerThread;
        workerThread = nullptr;
    }

    if (deviceFileDescriptor >= 0) {
        ::close(deviceFileDescriptor);
        deviceFileDescriptor = -1;

        setState(QCanBusDevice::UnconnectedState);
    }
}

void LonganI2CCanBusDevice::setConfigurationParameter(ConfigurationKey key, const QVariant &value) {
    // ponytail: stored in the base and applied in open(); set BitRateKey before
    // connectDevice() (Qt's documented order) or it takes effect on reconnect.
    QCanBusDevice::setConfigurationParameter(key, value);
}

bool LonganI2CCanBusDevice::writeFrame(const QCanBusFrame &frame) {
    if (state() != ConnectedState) {
        return false;
    }

    // Hand off to the worker thread's outgoing queue; the worker owns the fd.
    enqueueOutgoingFrame(frame);
    return true;
}

bool LonganI2CCanBusDevice::writeFrameToBus(const QCanBusFrame &frame) {
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
    return QString();  // TODO
}

void LonganI2CCanBusDevice::readLoop() {
    while (!QThread::currentThread()->isInterruptionRequested()) {
        int written = 0;
        while (hasOutgoingFrames()) {
            if (writeFrameToBus(dequeueOutgoingFrame())) {
                ++written;
            } else {
                cwarning << "Failed to write CAN frame";
            }
        }
        if (written > 0) {
            emit framesWritten(written);
        }

        uint8_t buffer[kReceiveBufferSize];
        const int n = ::read(deviceFileDescriptor, buffer, sizeof(buffer));
        if (n > 3) {
            const int len = buffer[2];
            if (len < 0 || len > kMaxFrameLen || n < 3 + len) {
                cwarning << "Malformed CAN frame: n=" << n << "len=" << len;
            }
            // ponytail: only draining the module's RX FIFO for now; when a consumer
            // needs speed data, build the frame and enqueueReceivedFrames({frame}).
            continue;
        }

        // Nothing available: brief bounded poll instead of spinning at 100% CPU.
        QThread::msleep(kPollIntervalMs);
    }
}

bool LonganI2CCanBusDevice::setRegister(unsigned char reg, QByteArray data) {
    data.prepend(reg);
    int transferedBytes = ::write(deviceFileDescriptor, data.constData(), data.size());
    return transferedBytes == data.size();
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
        case 31200:  // from docs
        case 31250:  // from #define name
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
