#include "networkService.h"

#include <QProcess>
#include <QStringList>
#include <QUuid>

NetworkService::NetworkService(SettingsManager &new_settings, QObject *parent)
    : QObject(parent), settings(new_settings) {
    connect(&accessPoint, &WifiAccessPoint::ready, this, &NetworkService::ready);
    connect(&accessPoint, &WifiAccessPoint::failed, this, &NetworkService::failed);
}

void NetworkService::ensureStarted() {
    if (accessPoint.isActive()) {
        emit ready();
        return;
    }

    QString ssid = settings.getWifiSsid();
    if (ssid.isEmpty()) {
        ssid = "AUTOAPP";
    }
    // ponytail: no stored key => per-session random WPA2 password (>=8 chars); the phone
    // gets it over BT anyway. Set wireless/key to pin your own.
    QString key = settings.getWifiKey();
    if (key.isEmpty()) {
        key = QUuid::createUuid().toString(QUuid::Id128).left(12);
    }

    accessPoint.start(ssid, key, detectWifiInterface());
}

QString NetworkService::detectWifiInterface() {
    QProcess process;
    process.start("nmcli", {"-t", "-f", "DEVICE,TYPE", "device", "status"});
    process.waitForFinished(5000);
    const QString out = QString::fromUtf8(process.readAllStandardOutput());
    for (const QString &line : out.split('\n', Qt::SkipEmptyParts)) {
        const QStringList parts = line.split(':');
        if (parts.size() >= 2 && parts[1] == "wifi") {
            return parts[0];
        }
    }
    return {};
}
