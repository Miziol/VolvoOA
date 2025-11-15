#include "CANService.h"

#include <QCanBus>
#include <QVariant>

#include "longanI2CCanBusDevice.h"

CANService::CANService(QObject *parent) : QObject(parent), category("CAN SERVICE") {
    screenTimer.setInterval(5000);
    screenTimer.callOnTimeout(this, &CANService::openScreen);

    lowSpeedCanBusDevice = new LonganI2CCanBusDevice(this);

    if (lowSpeedCanBusDevice->connectDevice()) {
        lowSpeedCanBusDevice->setConfigurationParameter(QCanBusDevice::BitRateKey,
                                                        QVariant(speedOfLowSpeedCan));
        screenTimer.start();
    } else {
        cerror << lowSpeedCanBusDevice->errorString();
    }
}

CANService::~CANService() {
    closeScreen();
}

void CANService::openScreen() {
    lowSpeedCanBusDevice->writeFrame(QCanBusFrame(
        HLDF_ID,
        QByteArray::fromRawData(reinterpret_cast<const char *>(HLDF_OPEN_MESSAGE), sizeof(HLDF_OPEN_MESSAGE))));
}

void CANService::closeScreen() {
    screenTimer.stop();

    lowSpeedCanBusDevice->writeFrame(QCanBusFrame(
        HLDF_ID,
        QByteArray::fromRawData(reinterpret_cast<const char *>(HLDF_CLOSE_MESSAGE), sizeof(HLDF_CLOSE_MESSAGE))));
}
