#ifndef AUTOAPP_GENERIC_AADEVICE_H
#define AUTOAPP_GENERIC_AADEVICE_H

#include <QObject>
#include <QString>

// ponytail: the shared interface of every Android Auto device entry the user can pick in the list
// (UsbDevice, BluetoothDevice). Picking one activates it (the transport-specific handshake); while
// picked it holds one transport channel, whose concrete form is its own: USB opens a libusb
// handle, Bluetooth adopts the dialed RFCOMM socket. Distinct from AndroidAutoDevice, which is a
// *running* projection session, not a pickable entry.
class GenericAaDevice : public QObject {
    Q_OBJECT

public:
    using QObject::QObject;

    Q_PROPERTY(QString displayName READ displayName CONSTANT)

    virtual QString displayName() const = 0;
    // Does this entry currently hold its transport channel?
    virtual bool isOpen() const = 0;

public slots:
    // Starts the transport-specific handshake: USB runs the AOAP dance, Bluetooth only records the
    // user's consent (the phone dials us).
    virtual void activate() = 0;
    // Releases the transport channel - twin of the acquisition done by activate()/open().
    virtual void close() = 0;
};

#endif  // AUTOAPP_GENERIC_AADEVICE_H
