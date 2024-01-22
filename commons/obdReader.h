#include "../libs/I2C_CAN_Arduino-main/Longan_I2C_CAN_Arduino.h"

class ObdReader {
    I2C_CAN *can;
public:
    ObdReader();
    
    requestPID();
}