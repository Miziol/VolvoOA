
#include "LinFrame.h"

class LinBus {
public:
    LinBus();

private:
    char *bytes;
    
    int frameNumber;
    LinFrame *frames;

public:
    bool raedBytes();
    bool frameAvailable();
    LinFrame getFrame();

private:
    void writeFrame();
};