#ifndef ANDROID_AUTO_SYSTEM_UPDATER_H
#define ANDROID_AUTO_SYSTEM_UPDATER_H

#include <qprocess.h>

#include <QObject>
#include <QRunnable>

#include "../logging/loggingCategory.h"

class SystemUpdater : public QObject, public QRunnable {
    Q_OBJECT
signals:
    void runningChanged(bool running);
    void updated();

public:
    Q_PROPERTY(bool running MEMBER m_running NOTIFY runningChanged);

public:
    SystemUpdater();
    ~SystemUpdater();

    void run();

private:
    QLoggingCategory category;

    bool m_running;
};

#endif  // ANDROID_AUTO_SYSTEM_UPDATER_H
