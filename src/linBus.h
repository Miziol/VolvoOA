#ifndef LIN_BUS_H
#define LIN_BUS_H

#include <SoftwareSerial.h>
#include <Vector.h>

#include "linFrame.h"

class LinBus {
    SoftwareSerial *softSerial;

    Vector<char> bytes;

    Vector<LinFrame> frames;

public:
    LinBus();
    LinBus(int rxPin, int txPin);

    void readBus();
    bool frameAvailable();
    LinFrame getFrame();

private:
    void analizeBytes();
};

#endif  // LIN_BUS_H
