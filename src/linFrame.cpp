#include <Streaming.h>

#include "linFrame.h"

LinFrame::LinFrame() {}

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

void LinFrame::printResponse() {
    Serial << "[";
    for (int i = 0; i < response.size(); i++) {
        Serial << _HEX (response[i]);
        if ( i < response.size() - 1) {
            Serial << ",";
        }
    }
    Serial << "]";
}

LinFrame& LinFrame::operator=(const LinFrame& other) {
    this->header = other.header;
    this->response = other.response;
    this->checksum = other.checksum;
}
