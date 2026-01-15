#ifndef LIN_FRAME_H
#define LIN_FRAME_H

#include <Vector.h>

class LinFrame {
    byte header;
    uint8_t responseSize;
    char response[8];
    char checksum;

public:
    LinFrame();
    LinFrame(const LinFrame &other) = default;
    LinFrame(byte header, Vector<byte> frame, byte checksum);

    byte getHeader();
    uint8_t getID();
    uint8_t getResponseSize();
    byte *getResponse();
    byte getChecksum();

    void printResponse();

    LinFrame &operator=(const LinFrame &other);
};

#endif  // LIN_FRAME_H
