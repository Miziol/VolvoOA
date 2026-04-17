#ifndef RTISTATE_H
#define RTISTATE_H
#include <Arduino.h>

#include "systemState.h"

class RTIState {
public:
    RTIState();

    void updateState(SystemState &systemState);

private:
    bool isPowerOn();
    void sendPowerButton();

    void sendButton(uint8_t code);
};

#endif  // RTISTATE_H
