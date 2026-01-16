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

#include <QKeyEvent>
#include <QObject>
#include <f1x/openauto/autoapp/Projection/IInputDevice.hpp>

#include "../../../../backend/settings/settingsManager.h"

namespace f1x {
namespace openauto {
namespace autoapp {
namespace projection {
class InputDevice : public QObject, public IInputDevice, boost::noncopyable {
    Q_OBJECT

public:
    InputDevice(SettingsManager &configuration,
                QObject *new_videoOutput,
                const QRect &touchscreenGeometry,
                const QRect &videoGeometry);

    void start(IInputDeviceEventHandler &eventHandler) override;
    void stop() override;
    QList<f1x::aasdk::proto::enums::ButtonCode::Enum> getSupportedButtonCodes() const override;
    bool hasTouchscreen() const override;
    QRect getTouchscreenGeometry() const override;

public slots:
    bool handleKeyEvent(int key, bool pressed);
    bool handleTouchEvent(qreal mouseX, qreal mouseY, QString type); // TODO change `type` to enum

private:
    QObject *videoOutput;
    SettingsManager &configuration_;
    QRect touchscreenGeometry_;
    QRect displayGeometry_;
    IInputDeviceEventHandler *eventHandler_;
    std::mutex mutex_;
};
} // namespace projection
} // namespace autoapp
} // namespace openauto
} // namespace f1x
