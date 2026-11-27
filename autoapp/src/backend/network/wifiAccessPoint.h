#ifndef AUTOAPP_WIFIACCESSPOINT_H
#define AUTOAPP_WIFIACCESSPOINT_H

#include <QObject>
#include <QString>
#include <functional>

#include "../logging/loggingCategory.h"

struct WifiApInfo {
    QString ssid;
    QString key;
    QString bssid;
    QString address;  // gateway IP the phone dials = us
};

// ponytail: brings up the hotspot via nmcli (this box has no libnm headers). All calls run
// asynchronously with chained QProcesses, so the GUI thread never blocks; ready()/failed()
// fire on the owning thread once the AP is up (or gave up).
class WifiAccessPoint : public QObject {
    Q_OBJECT

public:
    explicit WifiAccessPoint(QObject *parent = nullptr);

    void start(const QString &ssid, const QString &password, const QString &ifname);

    WifiApInfo info() const { return apInfo; }
    bool isActive() const { return active; }

signals:
    void ready();
    void failed(QString reason);

private:
    // nmcli steps run strictly in order; a fatal step aborts the sequence and reports the error.
    struct Step {
        QStringList args;
        bool fatal;
    };

    void runNmcliAsync(const QStringList &args, const std::function<void(int, QString)> &callback);
    void runNextStep();
    void onStepsFinished();

    QLoggingCategory category;

    bool active = false;
    WifiApInfo apInfo;
    QString pendingSsid;
    QString pendingKey;
    QString pendingIface;
    QList<Step> steps;
    int stepIndex = 0;
    QString lastOutput;
};

#endif  // AUTOAPP_WIFIACCESSPOINT_H
