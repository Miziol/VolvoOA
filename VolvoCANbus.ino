#include "commons/canBus.h"

CanBus can(0x25);

bool validCoolantTemp, validOilTemp;
int coolantTemp, oilTemp;

void printTemps();

void setup() {
    // TODO INIT LINBUS
    
    // INIT CANBUS
    can.init(CAN_250KBPS);
}

void loop() {
    // TODO LINBUS

    // TODO CANBUS
    validCoolantTemp = can.getCoolantTemp(&coolantTemp);
    validOilTemp = can.getOilTemp(&oilTemp);

    printTemps();
}

void printTemps() {
    Serial.print("Coolant temp: ");
    if(validCoolantTemp)
        Serial.print(coolantTemp);
    else
        Serial.print("--");
    Serial.print("°C\t\tOil temp: ");
    if(validOilTemp)
        Serial.print(oilTemp);
    else
        Serial.print("--");
    Serial.println("°C");
}
