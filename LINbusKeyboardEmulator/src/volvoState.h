#ifndef VOLVO_STATE_H
#define VOLVO_STATE_H

#include "Keyboard.h"

#define DEBUG_SWM 0
#define DEBUG_LSM 0

#define SWM_ID 0x20
#define LSM_ID 0x10

#define BACK_BUTTON KEY_HOME
#define ENTER_BUTTON KEY_RETURN
#define NEXT_BUTTON 176
#define MUTE_BUTTON 179
#define MIC_BUTTON KEY_INSERT
#define PREVIOUS_BUTTON 177

#define NEXT_ITEM_BUTTON KEY_PAGE_DOWN
#define PREVIOUS_ITEM_BUTTON KEY_PAGE_UP

class VolvoState {
    byte scrollWheelLastValue;
    int n;
    bool isDown;

public:
    void updateStateSWM(const byte* bytes);
    void updateStateLSM(const byte* bytes);

private:
    void sendButtonState(int button, bool isPressed);

    void sendNextItem();
    void sendPreviouseItem();
};

#endif // VOLVO_STATE_H