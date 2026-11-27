#ifndef AUTOAPP_AADEVICEENTRY_H
#define AUTOAPP_AADEVICEENTRY_H

#include <QObject>
#include <QString>

// The shared interface of every Android Auto *entry* the user can pick in the device list
// (UsbDevice, BluetoothDevice) - a candidate, not a live connection. One vocabulary throughout:
// the user calls activate() to start it, `active` says whether it is currently engaged, and
// deactivate() ends it. Distinct from AndroidAutoDevice, which is a *running* projection session
// (the result of an entry's handshake), not a pickable row.
class AaDeviceEntry : public QObject {
    Q_OBJECT

public:
    using QObject::QObject;

    // How the entry reaches its phone; lets the list pick a USB vs Wireless icon.
    enum class Type { Usb, Wireless };
    Q_ENUM(Type)

    virtual QString displayName() const = 0;
    // The transport of this entry. Constant per subclass.
    virtual Type type() const = 0;
    // Is this entry currently engaged (holds its transport channel)? Note this is the *transport*
    // resource, not "the user picked this row": for USB it is true from discovery, see UsbDevice.
    virtual bool isActive() const = 0;

    Q_PROPERTY(QString displayName READ displayName CONSTANT)
    Q_PROPERTY(Type type READ type CONSTANT)
    Q_PROPERTY(bool active READ isActive NOTIFY activeChanged)

signals:
    // Emitted whenever isActive() flips; subclasses must fire it from their acquire/release paths.
    void activeChanged();

public slots:
    // The list-entry "start" action: USB runs the AOAP handshake, Bluetooth records the user's
    // consent (the phone dials us). The start half of the activate()/active/deactivate() triad.
    virtual void activate() = 0;
    // Ends this entry's engagement and releases its transport resources. The stop half of the triad.
    virtual void deactivate() = 0;
};

#endif  // AUTOAPP_AADEVICEENTRY_H
