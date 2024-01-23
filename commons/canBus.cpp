#include "canBus.h"

CanBus::CanBus(unsigned char address) : can(address) {
    ;
}

void CanBus::init(unsigned char speed) {
    while (CAN_OK != can.begin(speed)) {
        Serial.println("CAN init fail, retry...");
        delay(100);
    }

    can.init_Mask(0, 0, 0x7FC);
    can.init_Mask(1, 0, 0x7FC);

    can.init_Filt(0, 0, 0x7E8);                 
    can.init_Filt(1, 0, 0x7E8);
    can.init_Filt(2, 0, 0x7E8);
    can.init_Filt(3, 0, 0x7E8);
    can.init_Filt(4, 0, 0x7E8); 
    can.init_Filt(5, 0, 0x7E8);
}

bool CanBus::getCoolantTemp(int *temp) {
    requestObdPid(PID_COOLANT_TEMP);

    return readTemp(temp);
}

bool CanBus::getOilTemp(int *temp) {
    requestObdPid(PID_OIL_TEMP);

    return readTemp(temp);
}

void CanBus::requestObdPid(unsigned char pid) {
    unsigned char tmp[8] = {0x02, 0x01, pid, 0, 0, 0, 0, 0};
    can.sendMsgBuf(CAN_ID_PID, 0, 8, tmp);
}

bool CanBus::readTemp(int *temp) {
    unsigned long timeout = millis();
    unsigned char len = 0;
    unsigned char buf[8];

    while(millis() - timeout < 1000)
    {
        if (CAN_MSGAVAIL == can.checkReceive()) {
            can.readMsgBuf(&len, buf);

            if(buf[1] == 0x41)
            {
                *temp = ((int) buf[3]) - 40;
                return 1;
            }
        }
    }

    return 0;
}
