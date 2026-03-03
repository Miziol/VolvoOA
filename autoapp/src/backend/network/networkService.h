#ifndef AUTOAPP_NETWORKSERVICE_H
#define AUTOAPP_NETWORKSERVICE_H
#include <QObject>

#include "../logging/loggingCategory.h"

class NetworkService : public QObject {
    Q_OBJECT

signals:
    void networkDevicesChanged();
    void connectTo();

public:
    Q_PROPERTY(QList<QObject *> networkDevices MEMBER devices NOTIFY networkDevicesChanged)

private:
    static inline int AA_PORT = 5277;

public:
    NetworkService();
    ~NetworkService();

public slots:
    void scanNetwork();

private:
    QLoggingCategory category;

    QList<QObject *> devices;

private:
    void lookForAA(const QString &ip);
    void addAADeviceToList(QString ip);
};

#endif  // AUTOAPP_NETWORKSERVICE_H
