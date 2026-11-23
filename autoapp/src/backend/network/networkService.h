#ifndef AUTOAPP_NETWORKSERVICE_H
#define AUTOAPP_NETWORKSERVICE_H

#include <QObject>

#include "../settings/settingsManager.h"
#include "wifiAccessPoint.h"

// ponytail: owns the network the head unit *hosts* for a phone client - today just the Wi-Fi
// AP used for wireless AA. The WPP handshake starts it (it must hand ssid/key to the phone);
// a future AA teardown must be able to drop it (single Wi-Fi radio). Kept out of
// AndroidAutoService: nmcli is no aasdk transport. Also the home for future host-side
// concerns (e.g. upstream connectivity) when they arrive.
class NetworkService : public QObject {
    Q_OBJECT

public:
    explicit NetworkService(SettingsManager &new_settings, QObject *parent = nullptr);

    void ensureStarted();  // idempotent; emits ready() at once if already up

    bool isAccessPointActive() const { return accessPoint.isActive(); }
    WifiApInfo info() const { return accessPoint.info(); }

signals:
    void ready();
    void failed(QString reason);

private:
    // The Wi-Fi card to host the AP on is resolved here and handed to the access point, which
    // only knows how to bring a named device up.
    static QString detectWifiInterface();

    SettingsManager &settings;
    WifiAccessPoint accessPoint;
};

#endif  // AUTOAPP_NETWORKSERVICE_H
