#ifndef ANDROID_AUTO_CANSERVICE_H
#define ANDROID_AUTO_CANSERVICE_H

#include <QCanBusDevice>
#include <QTimer>

#include "../logging/loggingCategory.h"

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
