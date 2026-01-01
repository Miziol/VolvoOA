#include "arduinoService.h"

ArduinoService::ArduinoService(QObject *parent)
    : QObject(parent), category("ARDUINO SERVICE"), currentArduinoIndex(-1), threadPool(QThreadPool::globalInstance()) {
    updater.setAutoDelete(false);

    lookForConnectedArduinos();
}

ArduinoService::~ArduinoService() {
    for (auto port : arduinos) {
        ((QSerialPort*) port)->close();
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
        currentArduinoIndex = 0;
        emit arduinoIndexChanged();
        cinfo << "Connected to" << portInfo.description() << "on:" << portInfo.systemLocation();
    } else {
        cwarning << "Failed to open port:" << portInfo.portName() << "describe as:" << portInfo.description()
                 << "with error:" << serialPort->errorString();
        delete serialPort;
    }
}

void ArduinoService::updateSelectedArduinoFirmware() {
    if (currentArduinoIndex < 0 || currentArduinoIndex >= arduinos.size()) {
        cerror << "Invalid arduino index";
        return;
    }
    updater.setTarget("arduino:avr:leonardo", "/dev/" + ((QSerialPort*) arduinos[currentArduinoIndex])->portName());
    threadPool->start(&updater);
}

void ArduinoService::receiveArduinoMessage() {
    for (const auto arduino : arduinos) {
        QSerialPort* port = ((QSerialPort *) arduino);
        while (port->canReadLine()) {
            QString line = port->readLine();
            if (currentArduinoIndex >= 0 && arduino == arduinos[currentArduinoIndex])
                newLineFromCurrentArduino(line);
            analizeLineContent(line);
        }
    }
}

void ArduinoService::analizeLineContent(QString line) {
    // TODO
}
