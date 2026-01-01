#include "arduinoUpdater.h"

#include <QFileInfo>
#include <QGuiApplication>

ArduinoUpdater::ArduinoUpdater() : category("ARDUINO UPDATER"), running(false) {}

ArduinoUpdater::~ArduinoUpdater() {}

void ArduinoUpdater::run() {
    if (running) {
        cerror << "Updater is running yet";
        return;
    }
    running = true;

    if (fqbn.isEmpty() || port.isEmpty()) {
        cerror << "Empty fqbn or port";
        return;
    }

    QFileInfo inoFile("../../LINbusKeyboardEmulator/LINbusKeyboardEmulator.ino");
    if (!inoFile.exists()) {
        cerror << "Could not find ino file:" << inoFile.absolutePath();
        return;
    }
    cinfo << "inoFile exists" << inoFile.absolutePath() << "with permission:" << inoFile.permissions();

    QString command = "arduino-cli compile --fqbn " + fqbn + " --port " + port + " --upload " + inoFile.absolutePath();
    QProcess process;

    cinfo << "Updating Arduino firmware with command:" << command;
    process.setWorkingDirectory(QGuiApplication::applicationDirPath());
    process.startCommand(command);
    process.waitForFinished(-1);

    if (process.exitCode() == 0) {
        cinfo << "Firmware update finished successfully";
        cinfo << process.readAllStandardOutput().split('\n');
    } else {
        cerror << "Firmware update failed";
        cerror << process.readAllStandardError();
    }

    running = false;
}

void ArduinoUpdater::setTarget(QString newFqbn, QString newPort) {
    if (running) {
        cerror << "Could change updater target, because process is running";
        return;
    }

    fqbn = newFqbn;
    port = newPort;
}
