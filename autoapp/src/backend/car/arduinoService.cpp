#include "arduinoService.h"

ArduinoService::ArduinoService(QObject *parent) : QObject(parent), category("ARDUINO SERVICE"), currentArduinoIndex(-1), threadPool(QThreadPool::globalInstance()) {
    lookForConnectedArduinos();
}

ArduinoService::~ArduinoService() {
    for (auto port : arduinos) {
        port->close();
        delete port;
    }
}

void ArduinoService::lookForConnectedArduinos() {
    for (const auto portInfo : QSerialPortInfo::availablePorts())
        if (portInfo.vendorIdentifier() == VENDOR_ARDUINO_SA)
            tryToConnectToArduino(portInfo);
}

void ArduinoService::tryToConnectToArduino(QSerialPortInfo portInfo) {
    QSerialPort *serialPort = new QSerialPort(portInfo);
    if (serialPort->open(QIODeviceBase::ReadOnly)) {
        connect(serialPort, &QSerialPort::readyRead, this, &ArduinoService::receiveArduinoMessage);
        arduinos.append(serialPort);
        cinfo << "Connected to" << portInfo.description() << "on:" << portInfo.systemLocation();
    } else {
        cwarning << "Failed to open port:" << portInfo.portName() << "describe as:" << portInfo.description()
                 << "with error:" << serialPort->errorString();
        delete serialPort;
    }
}

void ArduinoService::updateSelectedArduinoFirmware() {
    updater.setTarget("arduino:uri:leonardo", "/dev/" + arduinos[currentArduinoIndex]->portName());
    threadPool->start(&updater);
}

void ArduinoService::receiveArduinoMessage() {
    for (const auto arduino : arduinos) {
        while (arduino->canReadLine()) {
            QString line = arduino->readLine();
            if (currentArduinoIndex >= 0 && arduino == arduinos[currentArduinoIndex])
                newLineFromCurrentArduino(line);
            analizeLineContent(line);
        }
    }
}

void ArduinoService::analizeLineContent(QString line) {
    // TODO
}

