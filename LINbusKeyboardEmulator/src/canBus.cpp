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

void CanBus::clearDIM() {
    unsigned char clear[8] = {0xE1, 0xFE, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    can.sendMsgBuf(DIM_MESSAGE_ID, 0, 8, clear);
}

void CanBus::sendTestMessage() {
    unsigned char part1[8] = {0xA7, 0x00, 0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20}; // "Hello "
    unsigned char part2[8] = {0x21, 0x57, 0x6f, 0x72, 0x6c, 0x64, 0x20, 0x20}; // "World  "
    unsigned char part3[8] = {0x22, 0x20, 0x20, 0x20, 0x56, 0x65, 0x72, 0x73}; // "   vers"
    unsigned char part4[8] = {0x67, 0x69, 0x6F, 0x6E, 0x20, 0x30, 0x2E, 0x31}; // "ion 0.1"
    can.sendMsgBuf(DIM_MESSAGE_ID, 0, 8, part1);
    can.sendMsgBuf(DIM_MESSAGE_ID, 0, 8, part2);
    can.sendMsgBuf(DIM_MESSAGE_ID, 0, 8, part3);
    can.sendMsgBuf(DIM_MESSAGE_ID, 0, 8, part4);
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
