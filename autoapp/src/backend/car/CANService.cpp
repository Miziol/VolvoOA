#include "CANService.h"

#include <QCanBus>

CANService::CANService(QObject *parent) : QObject(parent), category("CAN SERVICE") {
    screenTimer.setInterval(5000);
    screenTimer.callOnTimeout(this, &CANService::openScreen);

    cerror << QCanBus::instance()->plugins().join(", ");

    lowSpeedCanBusDevice = QCanBus::instance()->createDevice("longan-i2c-canbus", "i2c-1");

    cerror << "START CONNECTING";

    if (lowSpeedCanBusDevice->connectDevice()) {
        screenTimer.start();
    } else {
        cerror << lowSpeedCanBusDevice->errorString();
    }

    cerror << "END CAN";
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
