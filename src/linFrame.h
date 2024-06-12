#ifndef LIN_FRAME_H
#define LIN_FRAME_H

#include <Vector.h>

class LinFrame {
    char header;
    int responseSize;
    char response[8];
    char checksum;
    
public:
    LinFrame();
    LinFrame(const LinFrame &other) = default;
    LinFrame(char header, Vector<char> frame, char checksum);

    char getHeader();
    char* getResponse();
    char getChecksum();

    void printResponse();

    LinFrame& operator=(const LinFrame& other);
};

#endif  // LIN_FRAME_H
