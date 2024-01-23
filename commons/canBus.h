#include "Longan_I2C_CAN_Arduino.h"

#define CAN_ID_PID          0x7DF
#define PID_COOLANT_TEMP    0x05
#define PID_OIL_TEMP        0x5C

class CanBus {
    I2C_CAN can;

public:
    CanBus(unsigned char address);

    void init(unsigned char speed);

    bool getCoolantTemp(int *temp);
    bool getOilTemp(int *temp);

private:
    void requestObdPid(unsigned char pid);
    bool readTemp(int *temp);

};
