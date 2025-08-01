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

#include <boost/asio.hpp>
#include <f1x/aasdk/USB/IUSBEndpoint.hpp>
#include <f1x/aasdk/USB/IUSBWrapper.hpp>
#include <memory>
#include <unordered_map>

namespace f1x {
namespace aasdk {
namespace usb {

class USBEndpoint : public IUSBEndpoint, public std::enable_shared_from_this<USBEndpoint>, boost::noncopyable {
public:
    USBEndpoint(IUSBWrapper &usbWrapper,
                boost::asio::io_service &ioService,
                libusb_device_handle *handle,
                uint8_t endpointAddress = 0x00);

    void controlTransfer(common::DataBuffer buffer, uint32_t timeout, Promise::Pointer promise) override;
    void bulkTransfer(common::DataBuffer buffer, uint32_t timeout, Promise::Pointer promise) override;
    void interruptTransfer(common::DataBuffer buffer, uint32_t timeout, Promise::Pointer promise) override;
    uint8_t getAddress() override;
    void cancelTransfers() override;
    libusb_device_handle *getDeviceHandle() const override;

private:
    typedef std::unordered_map<libusb_transfer *, Promise::Pointer> Transfers;

    using std::enable_shared_from_this<USBEndpoint>::shared_from_this;
    void transfer(libusb_transfer *transfer, Promise::Pointer promise);
    static void transferHandler(libusb_transfer *transfer);

    IUSBWrapper &usbWrapper_;
    boost::asio::io_service::strand strand_;
    libusb_device_handle *handle_;
    uint8_t endpointAddress_;
    Transfers transfers_;
    std::shared_ptr<USBEndpoint> self_;
};

}  // namespace usb
}  // namespace aasdk
}  // namespace f1x
