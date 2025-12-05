#ifndef ANDROID_AUTO_ARDUINOUPDATER_H
#define ANDROID_AUTO_ARDUINOUPDATER_H

#include <qprocess.h>

#include <QObject>
#include <QRunnable>

#include "../logging/loggingCategory.h"

class ArduinoUpdater : public QObject, public QRunnable {
    Q_OBJECT
signals:
    void outputChanged();

public:
    ArduinoUpdater();
    ~ArduinoUpdater();

    void run();

public slots:
    void setTarget(QString newFqbn, QString newPort);

private:
    QLoggingCategory category;

    QString fqbn, port;
    QProcess process;

    QStringList output;

private slots:
    void newLine();
};

#endif  // ANDROID_AUTO_ARDUINOUPDATER_H
