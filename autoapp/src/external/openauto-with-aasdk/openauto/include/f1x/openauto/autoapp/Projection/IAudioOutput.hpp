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

#include <f1x/aasdk/Common/Data.hpp>
#include <f1x/aasdk/Messenger/Timestamp.hpp>
#include <memory>

namespace f1x {
namespace openauto {
namespace autoapp {
namespace projection {

class IAudioOutput {
public:
    typedef std::shared_ptr<IAudioOutput> Pointer;

    IAudioOutput() = default;
    virtual ~IAudioOutput() = default;

    virtual bool open() = 0;
    virtual void write(aasdk::messenger::Timestamp::ValueType timestamp,
                       const aasdk::common::DataConstBuffer &buffer) = 0;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void suspend() = 0;
    virtual uint32_t getSampleSize() const = 0;
    virtual uint32_t getChannelCount() const = 0;
    virtual uint32_t getSampleRate() const = 0;
};

}  // namespace projection
}  // namespace autoapp
}  // namespace openauto
}  // namespace f1x
