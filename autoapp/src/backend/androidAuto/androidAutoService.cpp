#include "androidAutoService.h"

#include "aaDeviceEntry.h"
#include "usbAndroidAutoDevice.h"
#include "wirelessAndroidAutoDevice.h"

AndroidAutoService::AndroidAutoService(SettingsManager &new_settings, boost::asio::io_context &new_ioService)
    : category("ANDROID AUTO SERVICE"), settingsManager(new_settings), ioService(new_ioService), aaDevice(nullptr) {
    connectionTimeout.setSingleShot(true);
    connectionTimeout.setInterval(60000);
    connect(&connectionTimeout, &QTimer::timeout, this, [this]() {
        cwarning << "Android Auto connection attempt timed out, freeing the slot";
        disconnectDevice();
    });
}

AndroidAutoService::~AndroidAutoService() {
    if (wirelessAcceptor != nullptr) {
        boost::system::error_code ec;
        wirelessAcceptor->close(ec);
    }
}

bool AndroidAutoService::connectDevice(QObject *device) {
    auto *source = qobject_cast<AaDeviceEntry *>(device);
    if (source == nullptr) {
        return false;
    }
    if (state != ConnectionState::Idle) {
        cwarning << "Android Auto slot busy, ignoring connect request";
        return false;
    }

    cinfo << "Device selected, reserving the Android Auto slot";
    setConnectionState(ConnectionState::Connecting);

    // Transport stays hidden behind the source object: the entry owns its own start sequence
    // (AOAP handshake for USB, pairing + WPP for wireless); we only gate the single slot.
    source->activate();
    return true;
}

void AndroidAutoService::disconnectDevice() {
    if (aaDevice != nullptr) {
        aaDevice->deleteLater();
        aaDevice = nullptr;
        emit aaDeviceChanged();
    }
    setConnectionState(ConnectionState::Idle);
}

void AndroidAutoService::setConnectionState(ConnectionState newState) {
    if (state == newState) {
        return;
    }
    state = newState;

    if (state == ConnectionState::Connecting) {
        connectionTimeout.start();
    } else {
        connectionTimeout.stop();
    }
    if (state == ConnectionState::Idle) {
        wirelessReady = false;
    }

    emit connectionStateChanged();
}

bool AndroidAutoService::isAABusy() const {
    return state != ConnectionState::Idle;
}

void AndroidAutoService::addUSBDevice(libusb_context *context, libusb_device *device) {
    cinfo << "New AA device start processing";
    installAndroidAutoDevice(new UsbAndroidAutoDevice(this, device, context, ioService, *androidAutoEntityFactory));
}

void AndroidAutoService::addWirelessDevice(f1x::aasdk::tcp::ITCPEndpoint::SocketPointer socket) {
    cinfo << "Wireless AA client connected on projection port";
    installAndroidAutoDevice(
        new WirelessAndroidAutoDevice(this, socket, ioService, *androidAutoEntityFactory));
}

void AndroidAutoService::installAndroidAutoDevice(AndroidAutoDevice *device) {
    // The slot must have been claimed by connectDevice(); anything else (stray hotplug, a second
    // dial while connected, a late dial after release) is dropped so only one AA entity runs.
    if (state != ConnectionState::Connecting) {
        cwarning << "Android Auto slot not claimed, device ignored";
        device->deleteLater();
        return;
    }

    // Runs on the GUI thread - the entity's video/audio QObjects must get GUI affinity
    // or their gstreamer timers cannot run (empty video). androidAutoEntity->start()
    // re-dispatches to its own io strand, so nothing blocks here.
    aaDevice = device;
    setConnectionState(ConnectionState::Connected);
    emit aaDeviceChanged();
    aaDevice->open();
    aaDevice->start();
}

void AndroidAutoService::acceptNextWirelessClient() {
    if (wirelessAcceptor == nullptr) {
        return;
    }

    // ponytail: the phone dials the gateway (us) once it joins the AP; the accepted
    // socket goes straight into the AA entity (canonical HU-as-AP topology).
    auto socket = std::make_shared<boost::asio::ip::tcp::socket>(ioService);
    wirelessAcceptor->async_accept(*socket, [this, socket](const boost::system::error_code &ec) {
        if (!ec) {
            QMetaObject::invokeMethod(
                this,
                [this, socket]() {
                    if (!wirelessReady) {
                        cinfo << "Ignoring projection connection before wireless handshake completed";
                        boost::system::error_code ignored;
                        socket->close(ignored);
                        return;
                    }
                    addWirelessDevice(socket);
                },
                Qt::QueuedConnection);
        } else if (ec != boost::asio::error::operation_aborted) {
            cerror << "Wireless accept error:" << QString::fromStdString(ec.message());
        }

        if (ec != boost::asio::error::operation_aborted) {
            acceptNextWirelessClient();
        }
    });
}

void AndroidAutoService::removeDevice(libusb_device *device) {
    auto *usb = dynamic_cast<UsbAndroidAutoDevice *>(aaDevice);
    if (usb != nullptr && device == usb->getDevice()) {
        cinfo << "Stoping AA device";
        usb->deleteLater();
        aaDevice = nullptr;
        emit aaDeviceChanged();
        emit focusOnAA(false);
        setConnectionState(ConnectionState::Idle);
    }
}

void AndroidAutoService::startWirelessServer() {
    boost::system::error_code ec;
    wirelessAcceptor = std::make_unique<boost::asio::ip::tcp::acceptor>(ioService);
    const boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), AA_PORT);

    wirelessAcceptor->open(endpoint.protocol(), ec);
    if (!ec) {
        wirelessAcceptor->set_option(boost::asio::ip::tcp::acceptor::reuse_address(true), ec);
    }
    if (!ec) {
        wirelessAcceptor->bind(endpoint, ec);
    }
    if (!ec) {
        wirelessAcceptor->listen(boost::asio::socket_base::max_listen_connections, ec);
    }
    if (ec) {
        cerror << "Failed to start wireless AA server on port" << AA_PORT << ":" << QString::fromStdString(ec.message());
        wirelessAcceptor.reset();
        return;
    }

    cinfo << "Wireless AA server listening on port" << AA_PORT;
    acceptNextWirelessClient();
}

void AndroidAutoService::armWirelessProjection() {
    if (state != ConnectionState::Connecting) {
        cinfo << "Ignoring wireless handshake: no pending connection";
        return;
    }
    if (!wirelessReady) {
        cinfo << "Wireless handshake completed, projection server armed";
    }
    wirelessReady = true;
}

void AndroidAutoService::startIOServiceWorkers(boost::asio::io_context &ioService,
                                               std::vector<std::thread> &threadPool) {
    auto ioServiceWorker = [&ioService]() { ioService.run(); };

    threadPool.emplace_back(ioServiceWorker);
    threadPool.emplace_back(ioServiceWorker);
    threadPool.emplace_back(ioServiceWorker);
    threadPool.emplace_back(ioServiceWorker);
}

void AndroidAutoService::createFactories(QObject *qmlRootObject) {
    serviceFactory = new f1x::openauto::autoapp::service::ServiceFactory(ioService, settingsManager, qmlRootObject);
    androidAutoEntityFactory = new f1x::openauto::autoapp::service::AndroidAutoEntityFactory(
        this, ioService, settingsManager, *serviceFactory);
}

bool AndroidAutoService::isAADeviceConnected() {
    return aaDevice != nullptr;
}

void AndroidAutoService::onAndroidAutoQuit() {
    // Called from the entity's io strand; marshal to the service thread so all aaDevice access is single-threaded.
    QMetaObject::invokeMethod(
        this,
        [this]() {
            if (aaDevice == nullptr) {
                setConnectionState(ConnectionState::Idle);
                return;
            }
            aaDevice->deleteLater();
            aaDevice = nullptr;
            emit aaDeviceChanged();
            setConnectionState(ConnectionState::Idle);
        },
        Qt::QueuedConnection);
}

void AndroidAutoService::setFocusOnAA(bool focus) {
    emit focusOnAA(focus);
}
