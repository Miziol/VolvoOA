#ifndef LIN_BUS_H
#define LIN_BUS_H

#include <SoftwareSerial.h>
#include <Vector.h>

#include "linFrame.h"

#define DEBUG_SWM 0
#define DEBUG_LSM 0

#define SYNC_BYTE 0x55

#define SWM_ID 0x20
#define LSM_ID 0x10

class LinBus {
    SoftwareSerial *softSerial;

    Vector<char> bytes;
    char bytesStorage[100];

    Vector<LinFrame> frames;
    LinFrame framesStorage[10];

public:
    LinBus();
    LinBus(int rxPin, int txPin);

    void readBus();
    bool frameAvailable();
    LinFrame popFrame();

    void clearEmptyBytes();
    int incommingFrameSize();

    static int sizeOfFrame(uint8_t id);
    static void analizeSteeringWheelFrame(const byte* bytes, uint8_t size);
    static void analizeLightFrame(const byte* bytes, uint8_t size);
    static void analizeCEM(const byte* bytes, uint8_t size);

private:
    void analizeBytes();
};

#endif  // LIN_BUS_H
