#include "rtiState.h"

#include <IRremote.hpp>

#define POWER_LED 0
#define IR_PIN 13

#define POWER_BUTTON 0x45
#define SOURCE_BUTTON 0x46
#define UP_BUTTON SOURCE_BUTTON
#define LEFT_BUTTON 0x43
#define MENU_BUTTON 0x44
#define CENTER_BUTTON MENU_BUTTON
#define RIGHT_BUTTON 0x47
#define MODE_BUTTON 0x40
#define BOTTOM_BUTTON MODE_BUTTON

RTIState::RTIState() {
    IrSender.begin(IR_PIN);
}

void RTIState::updateState(SystemState &systemState) {
    switch (systemState) {
        case ACTIVE:
            if (!isPowerOn()) {
                sendPowerButton();
            }
            break;
        case STARTING:
        case STOPPING:
        case STOPPED:
            if (isPowerOn()) {
                sendPowerButton();
            }
    }
}

bool RTIState::isPowerOn() {
    return digitalRead(POWER_LED);
}

void RTIState::sendPowerButton() {
    sendButton(POWER_BUTTON);
}

void RTIState::sendButton(uint8_t button) {
    IrSender.sendNEC(0x00, button, 0);
}