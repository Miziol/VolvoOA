#ifndef ANDROID_AUTO_LONGANI2CCANBUSPLUGIN_H
#define ANDROID_AUTO_LONGANI2CCANBUSPLUGIN_H

#include <QtSerialBus/QCanBusFactory>

class LonganI2CCanBusPlugin : public QObject, public QCanBusFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QCanBusFactoryInterface" FILE "longan_i2c_canbus.json")
    Q_INTERFACES(QCanBusFactory)

public:
    QCanBusDevice *createDevice(const QString &interfaceName, QString *errorMessage) const override;
    QList<QCanBusDeviceInfo> availableDevices(QString *errorMessage) const override;
};

#endif  // ANDROID_AUTO_LONGANI2CCANBUSPLUGIN_H
