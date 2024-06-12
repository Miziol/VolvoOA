#include <Streaming.h>

#include "linFrame.h"

LinFrame::LinFrame() {}

LinFrame::LinFrame(char new_header, Vector<char> new_response, char new_checksum) {
    header = new_header;
    responseSize = new_response.size();
    for (int i = 0; i < new_response.size(); i++) {
        response[i] = new_response[i];
    }
    checksum = new_checksum;
}

char LinFrame::getHeader() {
    return header;
}

char* LinFrame::getResponse() {
    return response;
}

char LinFrame::getChecksum() {
    return checksum;
}

void LinFrame::printResponse() {
    Serial.print("[");
    for (int i = 0; i < responseSize; i++) {
        Serial.print(response[i], HEX);
        if ( i < responseSize - 1) {
            Serial.print(",");
        }
    }
    Serial.print("]");
}

LinFrame& LinFrame::operator=(const LinFrame& other) {
    this->header = other.header;
    this->responseSize = other.responseSize;
    for (int i = 0; i < responseSize; i++) {
        this->response[i] = other.response[i];
    }
    this->checksum = other.checksum;
}
