#include "networkService.h"

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

    accessPoint.start(ssid, key, settings.getWifiInterface());
}
