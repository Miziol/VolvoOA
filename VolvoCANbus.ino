#include <Arduino.h>
#include <Streaming.h>

#include "src/canBus.h"
#include "src/keyboardEmulator.h"
#include "src/linBus.h"
#include "src/linFrame.h"

#define TX_PIN 9
#define RX_PIN 8

CanBus can(0x25);
LinBus lin(RX_PIN, TX_PIN);

bool validCoolantTemp, validOilTemp;
int coolantTemp, oilTemp;

void printTemps();

void setup() {
    Serial.begin(9600);
    // INIT LINBUS
    
    // INIT CANBUS
    can.init(CAN_250KBPS);
}

void loop() {
    // LINBUS
    lin.readBus();
    while (lin.frameAvailable()) {
        LinFrame frame = lin.popFrame();

        Serial << "Header: " << frame.getHeader() << " response: " << frame.getResponse() << " with checksum: " << frame.getChecksum();

        if (frame.getHeader() == 0x20) {
            Serial.print("Some steering wheel frame");
        }
    }

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
