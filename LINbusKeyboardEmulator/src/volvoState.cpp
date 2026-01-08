#include <Streaming.h>

#include "volvoState.h"

void VolvoState::updateStateSWM(const byte *bytes) {
    //Buttons
    sendButtonState(BACK_BUTTON, (bool)(bytes[1] & 0b01000000));
    sendButtonState(ENTER_BUTTON, (bool)(bytes[1] & 0b00100000));
    sendButtonState(NEXT_BUTTON, (bool)(bytes[1] & 0b00010000));
    //sendButtonState(MUTE_BUTTON, (bool) (bytes[1] & 0b00000100)); // TODO
    sendButtonState(MIC_BUTTON, (bool)(bytes[1] & 0b00000100));
    sendButtonState(PREVIOUS_BUTTON, (bool)(bytes[1] & 0b00000010));

    if (bytes[1] & 0b10000000) {
        // Sound down
        if (DEBUG_SWM)
            Serial << "Sound down" << "\t";
    }
    if (bytes[1] & 0b01000000) {
        // Back
        if (DEBUG_SWM)
            Serial << "Back" << "\t";
    }
    if (bytes[1] & 0b00100000) {
        // Enter
        if (DEBUG_SWM)
            Serial << "Enter" << "\t";
    }
    if (bytes[1] & 0b00010000) {
        // Next
        if (DEBUG_SWM)
            Serial << "Next" << "\t";
    }
    if (bytes[1] & 0b00000100) {
        // Mute/Mic
        if (DEBUG_SWM)
            Serial << "Mute/Mic" << "\t";
    }
    if (bytes[1] & 0b00000010) {
        // Previous
        if (DEBUG_SWM)
            Serial << "Previous" << "\t";
    }
    if (bytes[2] & 0b00000001) {
        // Sound up
        if (DEBUG_SWM)
            Serial << "Sound up" << "\t";
    }

    // Scroll
    if ((bytes[2] & 0b01111110) != (scrollWheelLastValue & 0b01111110)) {
        if (n == 0) {
            // START ROTATION
            if (bytes[3] & 0b00000001) {
                isDown = true;
            } else {
                isDown = false;
            }
        } else {
            // NEXT ROTATION
            ;
        }
        scrollWheelLastValue = bytes[2];
        n++;
    } else if (n != 0) {
        // END ROTATION
        n--;
        for (int i = 0; i < n; i++) {
            if (isDown) {
                sendNextItem();
                if (DEBUG_SWM) {
                    Serial << "Scroll down" << "\t";
                }
            } else {
                sendPreviousItem();
                if (DEBUG_SWM) {
                    Serial << "Scroll up" << "\t";
                }
            }
        }
        n = 0;
    }
}

void VolvoState::updateStateLSM(const byte *bytes) {
    uint8_t brightness = bytes[0] & 0b00001111;
    if (DEBUG_LSM)
        Serial << "Brightness: " << brightness << endl;
}

void VolvoState::sendButtonState(int button, bool isPressed) {
    if (isPressed) {
        Keyboard.press(button);
    } else {
        Keyboard.release(button);
    }
}

void VolvoState::sendNextItem() {
    Keyboard.write(NEXT_ITEM_BUTTON);
}

void VolvoState::sendPreviousItem() {
    Keyboard.write(PREVIOUS_ITEM_BUTTON);
}
