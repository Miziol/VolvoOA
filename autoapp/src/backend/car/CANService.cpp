#include "CANService.h"

#include <QCanBus>

#include "longanI2CCanBusDevice.h"

CANService::CANService(QObject *parent) : QObject(parent), category("CAN SERVICE") {
    screenTimer.setInterval(5000);
    screenTimer.callOnTimeout(this, &CANService::openScreen);

    lowSpeedCanBusDevice = new LonganI2CCanBusDevice(this);

    if (lowSpeedCanBusDevice->connectDevice()) {
        screenTimer.start();
    } else {
        cerror << lowSpeedCanBusDevice->errorString();
    }
}

CANService::~CANService() {
    closeScreen();
}

void CANService::openScreen() {
    // TODO open command
}

void CANService::closeScreen() {
    screenTimer.stop();

    // TODO close command
}
