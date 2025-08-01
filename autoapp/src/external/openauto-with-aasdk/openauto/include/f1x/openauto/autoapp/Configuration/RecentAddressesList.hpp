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

#include <deque>
#include <f1x/openauto/autoapp/Configuration/IRecentAddressesList.hpp>

namespace f1x {
namespace openauto {
namespace autoapp {
namespace configuration {

class RecentAddressesList : public IRecentAddressesList {
public:
    RecentAddressesList(size_t maxListSize);

    void read() override;
    void insertAddress(const std::string &address) override;
    RecentAddresses getList() const override;

private:
    void load();
    void save();

    size_t maxListSize_;
    RecentAddresses list_;

    static const std::string cConfigFileName;
    static const std::string cRecentEntiresCount;
    static const std::string cRecentEntryPrefix;
};

}  // namespace configuration
}  // namespace autoapp
}  // namespace openauto
}  // namespace f1x
