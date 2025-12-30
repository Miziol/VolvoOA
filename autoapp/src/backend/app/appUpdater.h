#ifndef ANDROID_AUTO_APP_UPDATER_H
#define ANDROID_AUTO_APP_UPDATER_H

#include <qprocess.h>

#include <QObject>
#include <QRunnable>

#include "../logging/loggingCategory.h"

class AppUpdater : public QObject, public QRunnable {
    Q_OBJECT
signals:
    void runningChanged(bool running);
    void stepChanged(int step);
    void updated();

public:
    Q_PROPERTY(bool running MEMBER m_running NOTIFY runningChanged);
    Q_PROPERTY(int step MEMBER m_step NOTIFY stepChanged);
    Q_PROPERTY(int steps READ getStepsNumber CONSTANT);

public:
    AppUpdater();
    ~AppUpdater();

    void run();

public slots:
    int getStepsNumber();

private:
    QLoggingCategory category;

    bool m_running;
    int m_step;
    QList<QString> commands;
};

#endif  // ANDROID_AUTO_APP_UPDATER_H
