/*
*  This file is part of aasdk library project.
*  Copyright (C) 2018 f1x.studio (Michal Szwaj)
*
*  aasdk is free software: you can redistribute it and/or modify
*  it under the terms of the GNU General Public License as published by
*  the Free Software Foundation; either version 3 of the License, or
*  (at your option) any later version.

*  aasdk is distributed in the hope that it will be useful,
*  but WITHOUT ANY WARRANTY; without even the implied warranty of
*  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*  GNU General Public License for more details.
*
*  You should have received a copy of the GNU General Public License
*  along with aasdk. If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

#include <libusb.h>

#include <f1x/aasdk/Error/Error.hpp>
#include <f1x/aasdk/IO/Promise.hpp>
#include <f1x/aasdk/USB/IUSBWrapper.hpp>
#include <functional>
#include <memory>

namespace f1x {
namespace aasdk {
namespace usb {

class IUSBHub {
public:
    typedef std::shared_ptr<IUSBHub> Pointer;
    typedef io::Promise<libusb_device_handle *> Promise;

    IUSBHub() = default;
    virtual ~IUSBHub() = default;

    virtual void start(Promise::Pointer promise) = 0;
    virtual void cancel() = 0;
};

}  // namespace usb
}  // namespace aasdk
}  // namespace f1x
