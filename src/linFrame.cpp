#include "linFrame.h"

LinFrame::LinFrame(char new_header, Vector<char> new_response, char new_checksum) {
    header = new_header;
    response = new_response;
    checksum = new_checksum;
}

char LinFrame::getHeader() {
    return header;
}

Vector<char> LinFrame::getResponse() {
    return response;
}

char LinFrame::getChecksum() {
    return checksum;
}
