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

signals :
    void arduinosChanged();
    void arduinoIndexChanged();
    void updaterRunningChanged();
    void newLineFromCurrentArduino(QString line);
    void closeRequestReceived();

    void piShutdownRequestReceived();

public:
    Q_PROPERTY(QStringList arduinosList READ getArduinosList NOTIFY arduinosChanged);
    Q_PROPERTY(int currentIndex MEMBER currentArduinoIndex NOTIFY arduinoIndexChanged);

    Q_PROPERTY(QObject* arduinoUpdater READ getUpdater CONSTANT);

private:
    static inline uint16_t VENDOR_ARDUINO_SA = 0x2341;

public:
    ArduinoService(QObject *parent = nullptr);
    ~ArduinoService();

public
slots :
    void lookForConnectedArduinos();
    void tryToConnectToArduino(QSerialPortInfo portInfo);

    QStringList getArduinosList();
    QObject *getUpdater();

    void updateSelectedArduinoFirmware();

    void receiveArduinoMessage();

private:
    QLoggingCategory category;

    QList<QSerialPort *> arduinos;
    int currentArduinoIndex;

    QThreadPool *threadPool;
    ArduinoUpdater updater;

private
slots :
    void analizeLineContent(QString line);
};

#endif  // ANDROID_AUTO_ARDUINOSERVICE_H
