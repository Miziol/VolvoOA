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

#pragma once

#include <boost/asio.hpp>
#include <f1x/aasdk/Transport/ITransport.hpp>
#include <f1x/openauto/autoapp/Service/IAndroidAutoEntityFactory.hpp>
#include <f1x/openauto/autoapp/Service/IServiceFactory.hpp>

#include "../../../../backend/settings/settingsManager.h"

namespace f1x {
namespace openauto {
namespace autoapp {
namespace service {

class AndroidAutoEntityFactory : public IAndroidAutoEntityFactory {
public:
    AndroidAutoEntityFactory(IAndroidAutoEntityEventHandler *handler,
                             boost::asio::io_service &ioService,
                             SettingsManager &configuration,
                             IServiceFactory &serviceFactory);

    IAndroidAutoEntity::Pointer create(aasdk::usb::IAOAPDevice::Pointer aoapDevice) override;
    IAndroidAutoEntity::Pointer create(aasdk::tcp::ITCPEndpoint::Pointer tcpEndpoint) override;

private:
    IAndroidAutoEntity::Pointer create(aasdk::transport::ITransport::Pointer transport);

    IAndroidAutoEntityEventHandler *eventHandler_;
    boost::asio::io_service &ioService_;
    SettingsManager &configuration_;
    IServiceFactory &serviceFactory_;
};

}  // namespace service
}  // namespace autoapp
}  // namespace openauto
}  // namespace f1x
