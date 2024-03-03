#include "keyboardEmulator.h"

#include <Keyboard.h>

KeyboardEmulator::KeyboardEmulator() {
    Keyboard.begin();
}

void KeyboardEmulator::sendShutdownCombination() {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_F4);
    Keyboard.releaseAll();
}
