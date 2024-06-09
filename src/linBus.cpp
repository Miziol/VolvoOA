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
}

bool LinBus::frameAvailable() {
    return frames.size();
}

LinFrame LinBus::popFrame() {
    LinFrame frame = frames[0];
    frames.remove(0);
    return frame;
}

void LinBus::analizeBytes() {
    char frameBeginIndex;

    for (int i = 0; i < bytes.size(); i++) {
        if (bytes[i] == SYNC_BYTE && bytes.size() - 1 == i) { // only SYNC_BYTE in current bytes
            bytes.clear();
            bytes.push_back(SYNC_BYTE);
        } else if (bytes[i] == SYNC_BYTE) { // SYNC_BYTE and something else
            char header = bytes[i+1];
            int responseLenght;
            switch (header >> 4) {
                case 3:
                    responseLenght == 8;
                    break;
                case 2:
                    responseLenght == 4;
                    break;
                default:
                    responseLenght = 2;
                    break;
            }

            if (bytes.size() > i + 1 + responseLenght + 1) { // i + header + response + checksum
                Vector<char> responseBytes;
                for (int j = i + 2; j < i + 2 + responseLenght; j++) {
                    responseBytes.push_back(bytes[j]);
                }

                frames.push_back(LinFrame(bytes[i + 1], responseBytes, bytes[i + 1 + responseLenght + 1]));
                i = i + 1 + responseLenght + 1;
            } else {
                Vector<char> frameBytes;
                for (int j = i; j < bytes.size(); j++) {
                    frameBytes.push_back(bytes[j]);
                }
                bytes.clear();
                bytes = frameBytes;
                break;
            }
        }
    }

    Serial << "Frames to analize:" << frames.size() << endl;
}
