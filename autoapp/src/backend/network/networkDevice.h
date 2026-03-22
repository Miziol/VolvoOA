#ifndef AUTOAPP_NETWORKDEVICE_H
#define AUTOAPP_NETWORKDEVICE_H

#include <QHostInfo>
#include <QObject>

#include "../logging/loggingCategory.h"

class NetworkDevice : public QObject {
    Q_OBJECT

public:
    Q_PROPERTY(QString displayName READ getDisplayName CONSTANT)

public:
    NetworkDevice(QHostInfo new_info);
    ~NetworkDevice();

public slots:
    QString getDisplayName();
    QString getIPAddress();

private:
    QLoggingCategory category;

    QHostInfo info;
};

#endif  // AUTOAPP_NETWORKDEVICE_H
