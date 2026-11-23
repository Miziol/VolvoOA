#include "wifiAccessPoint.h"

#include <QFile>
#include <QProcess>
#include <QStringList>

namespace {
// ponytail: one fixed NM connection profile, recreated on every start so the
// credentials always match our settings. A fixed name makes the read-back easy.
const QLatin1String HOTSPOT_CONNECTION("AUTOAPP-Hotspot");
}  // namespace

WifiAccessPoint::WifiAccessPoint(QObject *parent) : QObject(parent), category("WIFI AP") {
    // ponytail: drop any hotspot left running by a previous session so the phone
    // cannot dial a stale projection session before our BT handshake runs.
    runNmcliAsync({"connection", "down", HOTSPOT_CONNECTION}, [](int, QString) {});
}

void WifiAccessPoint::start(const QString &ssid, const QString &password, const QString &ifname) {
    if (active) {
        emit ready();
        return;
    }

    pendingSsid = ssid;
    pendingKey = password;
    pendingIface = ifname;
    if (pendingIface.isEmpty()) {
        emit failed("No Wi-Fi interface provided");
        return;
    }

    cinfo << "Bringing up access point on" << pendingIface << "ssid" << pendingSsid;

    // Drop a stale profile first (both best-effort), then create + activate a fresh one and read
    // back the gateway IP; the remaining steps are fatal.
    steps = {
        {{"connection", "down", HOTSPOT_CONNECTION}, false},
        {{"connection", "delete", HOTSPOT_CONNECTION}, false},
        {{"device", "wifi", "hotspot", "con-name", HOTSPOT_CONNECTION, "ifname", pendingIface, "ssid",
          pendingSsid, "password", pendingKey},
         true},
        // ponytail: keep the profile from auto-starting on next boot and knocking out the normal
        // station connection.
        {{"connection", "modify", HOTSPOT_CONNECTION, "connection.autoconnect", "no"}, true},
        {{"-f", "IP4.ADDRESS", "device", "show", pendingIface}, true},
    };
    stepIndex = 0;
    runNextStep();
}

void WifiAccessPoint::runNextStep() {
    if (stepIndex >= steps.size()) {
        onStepsFinished();
        return;
    }

    const Step step = steps.at(stepIndex);
    runNmcliAsync(step.args, [this, step](int code, QString out) {
        lastOutput = out;
        if (code != 0 && step.fatal) {
            emit failed("nmcli " + step.args.join(' ') + " failed: " + out.trimmed());
            return;
        }
        ++stepIndex;
        runNextStep();
    });
}

void WifiAccessPoint::onStepsFinished() {
    for (const QString &line : lastOutput.split('\n', Qt::SkipEmptyParts)) {
        const QString value = line.section(':', 1).section('/', 0, 0).trimmed();
        if (!value.isEmpty()) {
            apInfo.address = value;
            break;
        }
    }
    QFile macFile("/sys/class/net/" + pendingIface + "/address");
    if (macFile.open(QIODevice::ReadOnly)) {
        apInfo.bssid = QString::fromUtf8(macFile.readAll()).trimmed();
    }
    apInfo.ssid = pendingSsid;
    apInfo.key = pendingKey;
    active = true;
    cinfo << "Access point up, phone dials" << apInfo.address;
    emit ready();
}

void WifiAccessPoint::runNmcliAsync(const QStringList &args,
                                    const std::function<void(int, QString)> &callback) {
    auto *process = new QProcess(this);
    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this,
            [process, callback](int code, QProcess::ExitStatus) {
                const QString out = QString::fromUtf8(process->readAllStandardOutput()) +
                                    QString::fromUtf8(process->readAllStandardError());
                process->deleteLater();
                callback(code, out);
            });
    connect(process, &QProcess::errorOccurred, this, [process, callback](QProcess::ProcessError error) {
        if (error == QProcess::FailedToStart) {
            process->deleteLater();
            callback(-1, "failed to start nmcli");
        }
    });
    process->start("nmcli", QStringList{"-t"} + args);
}
