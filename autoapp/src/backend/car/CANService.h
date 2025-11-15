#ifndef ANDROID_AUTO_CANSERVICE_H
#define ANDROID_AUTO_CANSERVICE_H

#include <QCanBusDevice>
#include <QTimer>

#include "../logging/loggingCategory.h"

const int speedOfLowSpeedCan = 125 * 1000;

const unsigned int HLDF_ID = 0x160;
const unsigned char HLDF_OPEN_MESSAGE[] = {0x75, 0x61, 0x2D, 0x3A, 0x77, 0x53, 0xB8, 0xF4};
const unsigned char HLDF_CLOSE_MESSAGE[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

class CANService : public QObject {
    Q_OBJECT

signals:

public:
    CANService(QObject *parent = nullptr);
    ~CANService();

private:
    QLoggingCategory category;

    QCanBusDevice *lowSpeedCanBusDevice;
    QTimer screenTimer;

public slots:
    void openScreen();
    void closeScreen();
};

#endif  // ANDROID_AUTO_CANSERVICE_H
