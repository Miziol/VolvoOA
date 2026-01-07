#include "appUpdater.h"

#include <qcoreapplication.h>
#include <QGuiApplication>

AppUpdater::AppUpdater() : category("APP UPDATER"), m_running(false), m_step(0), commands(QStringList() << "git pull" << "bash ../../install_requirements.sh" << "cmake .." << "make") {}

AppUpdater::~AppUpdater() {}

void AppUpdater::run() {
    if (m_running) {
        cerror << "Updater is running yet";
        return;
    }
    m_step = 0;
    emit stepChanged(m_step);
    m_running = true;
    emit runningChanged(m_running);

    QProcess process;

    qWarning() << process.workingDirectory() << QGuiApplication::applicationDirPath();

    for (QString& command : commands)
    {
        cinfo << "Processing step:" << command;
        process.startCommand(command);
        process.waitForFinished(-1);

        if (process.exitCode() == 0) {
            cinfo << "Step finished successfully";
            cinfo << process.readAllStandardOutput().split('\n');
        } else {
            cerror << "Step failed";
            cerror << process.readAllStandardError().split('\n');
            break;
        }

        m_step++;
        emit stepChanged(m_step);
    }

    m_running = false;
    emit runningChanged(m_running);
}

int AppUpdater::getStepsNumber()
{
    return commands.size();
}
