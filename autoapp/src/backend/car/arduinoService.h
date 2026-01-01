#ifndef ANDROID_AUTO_ARDUINOSERVICE_H
#define ANDROID_AUTO_ARDUINOSERVICE_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QThreadPool>

#include "../logging/loggingCategory.h"
#include "arduinoUpdater.h"

class ArduinoService : public QObject {
    Q_OBJECT

signals:
    void arduinosChanged();
    void arduinoIndexChanged();
    void newLineFromCurrentArduino(QString line);
    void closeRequestReceived();

public:
    Q_PROPERTY(QList<QObject*> arduinos MEMBER arduinos NOTIFY arduinosChanged);
    Q_PROPERTY(int currentIndex MEMBER currentArduinoIndex NOTIFY arduinoIndexChanged);

private:
    static inline uint16_t VENDOR_ARDUINO_SA = 0x2341;

public:
    ArduinoService(QObject *parent = nullptr);
    ~ArduinoService();

public slots:
    void lookForConnectedArduinos();
    void tryToConnectToArduino(QSerialPortInfo portInfo);

    void updateSelectedArduinoFirmware();

    void receiveArduinoMessage();

private:
    QLoggingCategory category;

    QList<QObject *> arduinos;
    int currentArduinoIndex;

    QThreadPool *threadPool;
    ArduinoUpdater updater;

private slots:
    void analizeLineContent(QString line);
};

#endif  // ANDROID_AUTO_ARDUINOSERVICE_H
