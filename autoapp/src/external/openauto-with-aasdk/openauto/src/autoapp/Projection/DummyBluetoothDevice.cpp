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

#include <f1x/openauto/autoapp/Projection/DummyBluetoothDevice.hpp>

namespace f1x {
namespace openauto {
namespace autoapp {
namespace projection {

void DummyBluetoothDevice::stop() {}

bool DummyBluetoothDevice::isPaired(const std::string &) const {
    return false;
}

void DummyBluetoothDevice::pair(const std::string &, PairingPromise::Pointer promise) {
    promise->reject();
}

std::string DummyBluetoothDevice::getLocalAddress() const {
    return "";
}

bool DummyBluetoothDevice::isAvailable() const {
    return false;
}

}  // namespace projection
}  // namespace autoapp
}  // namespace openauto
}  // namespace f1x
