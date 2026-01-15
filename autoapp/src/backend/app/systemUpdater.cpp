#include "systemUpdater.h"

SystemUpdater::SystemUpdater() : category("SYSTEM UPDATER"), m_running(false) {}

SystemUpdater::~SystemUpdater() {}

void SystemUpdater::run() {
    if (m_running) {
        cerror << "Updater is running yet";
        return;
    }
    m_running = true;
    emit runningChanged(m_running);

    QString command = "apt update";
    QProcess process;

    cinfo << "Updating System packages:" << command;
    process.startCommand(command);
    process.waitForFinished(-1);

    if (process.exitCode() == 0) {
        cinfo << "System update finished successfully";
        cinfo << process.readAllStandardOutput().split('\n');
    } else {
        cerror << "System update failed";
        cerror << process.readAllStandardError().split('\n');
    }

    command = "apt upgrade -y";

    cinfo << "Upgrading System packages:" << command;
    process.startCommand(command);
    process.waitForFinished(-1);

    if (process.exitCode() == 0) {
        cinfo << "System upgrade finished successfully";
        cinfo << process.readAllStandardOutput().split('\n');
    } else {
        cerror << "System upgrade failed";
        cerror << process.readAllStandardError().split('\n');
    }

    m_running = false;
    emit runningChanged(m_running);
}
