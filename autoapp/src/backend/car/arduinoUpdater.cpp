#include "arduinoUpdater.h"

#include <QGuiApplication>

ArduinoUpdater::ArduinoUpdater() : category("ARDUINO UPDATER") {
    connect(&process, &QProcess::readyReadStandardOutput, this, &ArduinoUpdater::newLine);
    connect(&process, &QProcess::readyReadStandardError, this, &ArduinoUpdater::newLine);
}

ArduinoUpdater::~ArduinoUpdater() {}

void ArduinoUpdater::run() {
    if (fqbn.isEmpty() || port.isEmpty()) {
        cerror << "Empty fqbn or port";
        return;
    }

    QString command = "arduino-cli compile --fqbn " + fqbn + " --port " + port + " --upload ../../LINbusKeyboardEmulator/LINbusKeyboardEmulator.ino";
    process.setWorkingDirectory(QGuiApplication::applicationDirPath());
    process.startCommand(command);
    process.waitForFinished(-1);

    if (process.exitCode() == 0) {
        cinfo << "Firmware update finished successfully";
    } else {
        cerror << "Firmware update failed";
    }
}

void ArduinoUpdater::setTarget(QString newFqbn, QString newPort) {
    if (process.state() != QProcess::NotRunning) {
        cerror << "Could change updater target, because process is running";
        return;
    }

    fqbn = newFqbn;
    port = newPort;

    output.clear();
    emit outputChanged();
}

void ArduinoUpdater::newLine() {
    output.append(process.readLine());
    emit outputChanged();
}
