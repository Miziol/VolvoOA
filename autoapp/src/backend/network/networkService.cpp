#include "networkService.h"

#include <QtConcurrent/qtconcurrentrun.h>

#include <QHostInfo>
#include <QNetworkInterface>
#include <QTcpSocket>
#include <QTimer>

#include "networkDevice.h"

NetworkService::NetworkService() : category("NETWORK SERVICE") {
    scanNetwork();
}

NetworkService::~NetworkService() {
    for (const auto &device : devices) {
        device->deleteLater();
        devices.removeOne(device);
    }
}

void NetworkService::scanNetwork() {
    QString localBase;
    for (const auto interface : QNetworkInterface::allInterfaces()) {
        for (const auto address : interface.addressEntries()) {
            if (!address.ip().isLinkLocal() && !address.ip().isLoopback() &&
                address.ip().protocol() == QAbstractSocket::IPv4Protocol) {
                quint32 netIP = address.ip().toIPv4Address() & address.netmask().toIPv4Address();

                for (int i = 1; i < ~address.netmask().toIPv4Address(); i++) {
                    QString ip = QHostAddress(netIP + i).toString();
                    QtConcurrent::run([this, ip]() { this->lookForAA(ip); });
                }
            }
        }
    }
}

void NetworkService::lookForAA(const QString &ip) {
    QTcpSocket socket;

    socket.connectToHost(ip, AA_PORT);

    if (socket.waitForConnected(1000)) {
        QMetaObject::invokeMethod(this, [this, ip]() { this->addAADeviceToList(ip); }, Qt::QueuedConnection);

        socket.abort();
    }
}

void NetworkService::addAADeviceToList(QString ip) {
    devices.append(new NetworkDevice(QHostInfo::fromName(ip)));
    emit networkDevicesChanged();
}
