#include "linBus.h"

LinBus::LinBus() {}

LinBus::LinBus(int rxPin, int txPin) {
    softSerial = new SoftwareSerial(rxPin, txPin);
}

void LinBus::readBus() {
    if (softSerial != nullptr) {
        while (Serial.available()) {
            bytes.push_back(Serial.read());
        }
    } else {
        while (softSerial->available()) {
            bytes.push_back(softSerial->read());
        }
    }
    
    analizeBytes();
}

bool LinBus::frameAvailable() {
    return frames.size();
}

LinFrame LinBus::getFrame() {
    LinFrame frame = frames[0];
    frames.remove(0);
    return frame;
}

void LinBus::analizeBytes() {
    // TODO convert bytes => frames
}
