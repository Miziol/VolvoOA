/*
*  This file is part of openauto project.
*  Copyright (C) 2018 f1x.studio (Michal Szwaj)
*
*  openauto is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 3 of the License, or
*  (at your option) any later version.

*  openauto is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with openauto. If not, see <http://www.gnu.org/licenses/>.
*/

#include <QApplication>
#include <QDebug>
#include <f1x/openauto/btservice/AndroidBluetoothServer.hpp>
#include <f1x/openauto/btservice/AndroidBluetoothService.hpp>

namespace btservice = f1x::openauto::btservice;

int main(int argc, char *argv[]) {
    QApplication qApplication(argc, argv);

    const QBluetoothAddress address;
    const uint16_t portNumber = 5000;

    btservice::AndroidBluetoothServer androidBluetoothServer;
    if (!androidBluetoothServer.start(address, portNumber)) {
        qCritical() << "[btservice] Server start failed.";
        return 2;
    }

    qInfo() << "[btservice] Listening for connections, address: " << address.toString().toStdString()
            << ", port: " << portNumber;

    btservice::AndroidBluetoothService androidBluetoothService(portNumber);
    if (!androidBluetoothService.registerService(address)) {
        qCritical() << "[btservice] Service registration failed.";
        return 1;
    } else {
        qInfo() << "[btservice] Service registered, port: " << portNumber;
    }

    qApplication.exec();
    androidBluetoothService.unregisterService();

    return 0;
}
