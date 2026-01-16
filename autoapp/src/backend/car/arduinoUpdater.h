#ifndef ANDROID_AUTO_ARDUINOUPDATER_H
#define ANDROID_AUTO_ARDUINOUPDATER_H

#include <qprocess.h>

#include <QObject>
#include <QRunnable>

#include "../logging/loggingCategory.h"

class ArduinoUpdater : public QObject, public QRunnable {
    Q_OBJECT

signals:
    void runningChanged();
    void outputChanged();

public:
    Q_PROPERTY(bool running MEMBER running NOTIFY runningChanged);

public:
    ArduinoUpdater();
    ~ArduinoUpdater();

    void run();

public slots:
    void setTarget(QString newFqbn, QString newPort);

private:
    QLoggingCategory category;

    bool running;
    QString fqbn, port;
};

#endif  // ANDROID_AUTO_ARDUINOUPDATER_H
