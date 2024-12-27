#ifndef KEYBOARD_EMULATOR_H
#define KEYBOARD_EMULATOR_H

class KeyboardEmulator {
public:
    KeyboardEmulator();

    void next();
    void previouse();

    void sendShutdownCombination();
};

#endif  // KEYBOARD_EMULATOR_H
