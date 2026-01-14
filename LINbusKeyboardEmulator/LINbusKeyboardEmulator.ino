#include <Arduino.h>
#include <Streaming.h>

#include "src/lin/linBus.h"
#include "src/lin/linFrame.h"
#include "src/systemState.h"
#include "src/volvoState.h"

#define TX_PIN 9
#define RX_PIN 8

#define DEBUG 0

unsigned long stoppingStartTime = 0; // TODO same ?
const unsigned long STOPPING_TIMEOUT = 60000;

LinBus lin(RX_PIN, TX_PIN);
SystemState systemState;
VolvoState state;

void updateState();

void setup() {
    Serial.begin(9600);
}

void loop() {
    lin.readBus();
    while (lin.frameAvailable()) {
        LinFrame frame = lin.popFrame();

        switch (frame.getID()) {
            case SWM_ID:
                state.updateStateSWM(frame.getResponse());
                break;
            case LSM_ID:
                state.updateStateLSM(frame.getResponse());
                break;
            default:
                if (DEBUG) {
                    Serial << "Header: " << _HEX((uint8_t)frame.getHeader())
                        << " ID: " << _HEX(frame.getID())
                        << " response: ";
                    frame.printResponse();
                    Serial << endl;
                }
        }
    }

    while (Serial.available()) {
        String message = Serial.readStringUntil('\n');
        if (message == "STARTED")
            systemState = ACTIVE;
    }

    updateState();
}

void updateState() {
    if (!lin.isActive()) {
        systemState = STOPPING;
        Serial << "SHUTDOWN" << endl;
    } else {
        switch (systemState) {
            case STOPPED:
                systemState = STARTING;
                state.setPhonePower(true);
                state.setScreenPower(true);
                state.sendStartPISignal();
                break;

            case STARTING:
                // EVERY THING IS OK
                break;

            case ACTIVE:
                // EVERY THING IS OK
                break;

            case STOPPING:
                if (millis() - stoppingStartTime > STOPPING_TIMEOUT) {
                    systemState = STOPPED;
                    state.setPhonePower(false);
                    state.setScreenPower(false);
                }
                break;
        }
    }
}
