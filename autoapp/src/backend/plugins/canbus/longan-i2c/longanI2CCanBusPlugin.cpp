#include "longanI2CCanBusPlugin.h"

#include <QVariant>

#include "longanI2CCanBusDevice.h"

QCanBusDevice *LonganI2CCanBusPlugin::createDevice(const QString &interfaceName, QString *errorMessage = nullptr) const {
    Q_UNUSED(errorMessage);
    auto *device = new LonganI2CCanBusDevice(); // TODO refactor constructor pass parameters
    device->setConfigurationParameter(static_cast<QCanBusDevice::ConfigurationKey>(QCanBusDevice::UserKey + 1), QVariant(interfaceName));
    device->setConfigurationParameter(static_cast<QCanBusDevice::ConfigurationKey>(QCanBusDevice::UserKey + 2), 0x25);
    return device;
}

QList<QCanBusDeviceInfo> availableDevices(QString *errorMessage) const {
    return ;
}
