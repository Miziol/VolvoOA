#ifndef LIN_FRAME_H
#define LIN_FRAME_H

#include <Vector.h>

class LinFrame {
    char header;
    Vector<char> response;
    char checksum;
    
public:
    LinFrame();
    LinFrame(char header, Vector<char> frame, char checksum);

    char getHeader();
    Vector<char> getResponse();
    char getChecksum();
};

#endif  // LIN_FRAME_H
