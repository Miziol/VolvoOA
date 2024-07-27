#include "keyboardEmulator.h"

#include <Keyboard.h>

KeyboardEmulator::KeyboardEmulator() {
    Keyboard.begin();
}

void KeyboardEmulator::next() {
    Keyboard.write(0xB0);
}

void KeyboardEmulator::previouse() {
    Keyboard.write(0xB1);
}

void KeyboardEmulator::sendShutdownCombination() {
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_SHIFT);
    Keyboard.press(KEY_F4);
    Keyboard.releaseAll();
}
