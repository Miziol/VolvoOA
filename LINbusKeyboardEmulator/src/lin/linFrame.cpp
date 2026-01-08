#include <Streaming.h>

#include "linFrame.h"

LinFrame::LinFrame() {
}

LinFrame::LinFrame(byte new_header, Vector<byte> new_response, byte new_checksum) {
    header = new_header;
    responseSize = new_response.size();
    for (int i = 0; i < new_response.size(); i++) {
        response[i] = new_response[i];
    }
    checksum = new_checksum;
}

byte LinFrame::getHeader() {
    return header;
}

uint8_t LinFrame::getID() {
    return header & 0b00111111;
}

uint8_t LinFrame::getResponseSize() {
    return responseSize;
}

byte *LinFrame::getResponse() {
    return response;
}

byte LinFrame::getChecksum() {
    return checksum;
}

void LinFrame::printResponse() {
    Serial.print("[");
    for (int i = 0; i < responseSize; i++) {
        Serial.print((uint8_t)response[i], BIN);
        if (i < responseSize - 1) {
            Serial.print(",");
        }
    }
    Serial.print("]");
}

LinFrame &LinFrame::operator=(const LinFrame &other) {
    this->header = other.header;
    this->responseSize = other.responseSize;
    for (int i = 0; i < responseSize; i++) {
        this->response[i] = other.response[i];
    }
    this->checksum = other.checksum;
}
