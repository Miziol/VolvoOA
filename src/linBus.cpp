#include <Streaming.h>

#include "linBus.h"

LinBus::LinBus() {}

LinBus::LinBus(int rxPin, int txPin) {
    softSerial = new SoftwareSerial(rxPin, txPin);
    softSerial->begin(9600);
    
    bytes.setStorage(bytesStorage);
    frames.setStorage(framesStorage);
}

void LinBus::readBus() {
    while (softSerial->available()) {
        bytes.push_back(softSerial->read());
    }

    analizeBytes();

    Serial << "After analize: ";
    frames[frames.size()-1].printResponse();
    Serial << endl;
}

bool LinBus::frameAvailable() {
    return frames.size();
}

LinFrame LinBus::popFrame() {
    LinFrame frame = frames[0];
    frames.remove(0);
    return frame;
}

void LinBus::clearEmptyBytes() {
    while (bytes.size()) {
        if (bytes[0] == 0) {
            bytes.remove(0);
        } else {
            break;
        }
    }
}

int LinBus::incommingFrameSize() {
    if (bytes.size() >= 2 && bytes[0] == SYNC_BYTE) {
        return sizeOfFrame(bytes[1]);
    }

    return 0;
}

int LinBus::sizeOfFrame(char id) {
    switch (id >> 4) {
        case 3:
            return 11; // sync + header + 8 + checksum
        case 2:
            return 7; // sync + header + 4 + checksum
    }
    return 5; // sync + header + 2 + checksum
}

void LinBus::analizeBytes() {
    while (bytes.size()) {
        clearEmptyBytes();
        int frameSize = incommingFrameSize();
        if (frameSize == 0) { // invalid frame begging or too low bytes
            if (bytes.size() < 2) { // too low bytes
                return;
            } else { // invalid frame
                while (bytes[0] != 0 && bytes.size()) {
                    bytes.remove(0);
                }
            }
        } else {
            if (bytes.size() < frameSize) { // frame incompleate
                return;
            } else {
                const int responseSize = frameSize - 3; // frameSize - sync - header - checksum
                char responseStorage[responseSize];
                Vector<char> response;
                response.setStorage(responseStorage, responseSize, 0);

                for (int i = 0; i < frameSize - 3; i++) {
                    response.push_back(bytes[2+i]);
                }
                frames.push_back(LinFrame(bytes[1], response, bytes[frameSize-1]));

                for (int i = 0; i < frameSize; i++) {
                    bytes.remove(0);
                }

                Serial << "Push back 2: " << frames.size()-1;
                frames[frames.size()-1].printResponse();
                Serial << endl;
            }
        }
                Serial << "BEfore while: " << frames.size()-1;
                frames[frames.size()-1].printResponse();
                Serial << endl;
    }

    Serial << "Frames to analize:" << frames.size() << endl;
}
