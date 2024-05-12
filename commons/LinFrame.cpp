#include "LinFrame.h"

LinFrame::LinFrame(char frame[]) {

}

char LinFrame::getHeader() {
    return header;
}

char* LinFrame::getResponse() {
    return response;
}

bool LinFrame::isValid() {
    return valid;
}
