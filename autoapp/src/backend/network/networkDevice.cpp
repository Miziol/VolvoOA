#include "networkDevice.h"

NetworkDevice::NetworkDevice(QHostInfo new_info) : category("NETWORK DEVICE"), info(new_info) {}

NetworkDevice::~NetworkDevice() {}

QString NetworkDevice::getDisplayName() {
    return info.hostName() + " (" + info.addresses().first().toString() + ")";
}

QString NetworkDevice::getIPAddress() {
    return info.addresses().first().toString();
}
