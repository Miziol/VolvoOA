#ifndef AUTOAPP_WIRELESSANDROIDAUTODEVICE_H
#define AUTOAPP_WIRELESSANDROIDAUTODEVICE_H

#include <libusb.h>

#include <QAudioFormat>
#include <QObject>
#include <QTimer>
#include <QtNetwork/QSslCertificate>
#include <QtNetwork/QSslKey>
#include <QtNetwork/QSslSocket>

#include "../../external/openauto-with-aasdk/openauto/include/f1x/openauto/autoapp/Service/AndroidAutoEntity.hpp"
#include "../logging/loggingCategory.h"
#include "f1x/aasdk/TCP/TCPWrapper.hpp"
#include "f1x/openauto/autoapp/Service/AndroidAutoEntityFactory.hpp"

class WirelessAndroidAutoDevice : public QObject {
    Q_OBJECT

signals:

public:
    WirelessAndroidAutoDevice(QObject *parent,
                        QString new_ipAddress,
                      boost::asio::io_service &new_ioService,
                      f1x::openauto::autoapp::service::AndroidAutoEntityFactory &new_androidAutoEntityFactory);
    ~WirelessAndroidAutoDevice();

private:
    QLoggingCategory category;

    f1x::aasdk::tcp::ITCPEndpoint::SocketPointer socket;
    f1x::aasdk::tcp::TCPWrapper tcpWrapper;
    boost::asio::io_service &ioService;
    f1x::openauto::autoapp::service::AndroidAutoEntityFactory &androidAutoEntityFactory;

    QString ipAddress;
    f1x::openauto::autoapp::service::IAndroidAutoEntity::Pointer androidAutoEntity;

public slots:
    void open();
    void close();
    void start();
    void stop();

private:


    void connectHandler(const boost::system::error_code& ec);
};

#endif  // AUTOAPP_WIRELESSANDROIDAUTODEVICE_H
