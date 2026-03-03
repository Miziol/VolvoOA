#include "networkService.h"

#include <QtConcurrent/qtconcurrentrun.h>

#include <QNetworkInterface>
#include <QTcpSocket>
#include <QTimer>

NetworkService::NetworkService() : category("NETWORK SERVICE") {
    scanNetwork();
}

NetworkService::~NetworkService() {}

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
    QTcpSocket *socket = new QTcpSocket();

    connect(socket, &QTcpSocket::connected, this, [this, socket, ip]() {
        cinfo << ">>> ZNALEZIONO ANDROID AUTO:" << ip;
        socket->abort();
        socket->deleteLater();
    }, Qt::QueuedConnection);

    connect(socket, &QTcpSocket::errorOccurred, this, [this, socket](QAbstractSocket::SocketError) {
        cerror << ">>> NO AA";
        socket->deleteLater();
    }, Qt::QueuedConnection);

    socket->connectToHost(ip, AA_PORT);

    QTimer::singleShot(1000, socket, [socket]() {
        if (socket && socket->state() != QAbstractSocket::ConnectedState) {
            qWarning() << "TIMEOUT";
            socket->abort();
            socket->deleteLater();
        }
    });
}
