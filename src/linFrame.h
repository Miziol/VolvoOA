#ifndef LIN_FRAME_H
#define LIN_FRAME_H

class LinFrame {
    bool valid;
    char header;
    char response[];
    
public:
    LinFrame(char frame[]);

    char getHeader();
    char* getResponse();

    bool isValid();
};

#endif  // LIN_FRAME_H
