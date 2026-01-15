#ifndef LIN_BUS_H
#define LIN_BUS_H

#include <SoftwareSerial.h>
#include <Vector.h>

#include "linFrame.h"

#define SYNC_BYTE 0x55

const unsigned long LIN_TIMEOUT = 15000;

class LinBus {
    SoftwareSerial *softSerial;

    unsigned long lastLinActivityTimestamp = 0;

    Vector<char> bytes;
    char bytesStorage[100];

    Vector<LinFrame> frames;
    LinFrame framesStorage[10];

public:
    LinBus();
    LinBus(int rxPin, int txPin);

    bool isActive();

    void readBus();
    bool frameAvailable();
    LinFrame popFrame();

private:
    void clearEmptyBytes();
    int incommingFrameSize();
    void analizeBytes();
};

#endif  // LIN_BUS_H
