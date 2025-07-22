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

#include <f1x/openauto/Common/Log.hpp>
#include <f1x/openauto/autoapp/Projection/IInputDeviceEventHandler.hpp>
#include <f1x/openauto/autoapp/Projection/InputDevice.hpp>

namespace f1x
{
namespace openauto
{
namespace autoapp
{
namespace projection
{

InputDevice::InputDevice(QObject& parent, configuration::IConfiguration::Pointer configuration, const QRect& touchscreenGeometry, const QRect& displayGeometry)
    : parent_(parent)
    , configuration_(std::move(configuration))
    , touchscreenGeometry_(touchscreenGeometry)
    , displayGeometry_(displayGeometry)
    , eventHandler_(nullptr)
{
    this->moveToThread(parent.thread());
}

void InputDevice::start(IInputDeviceEventHandler& eventHandler)
{
    std::lock_guard<decltype(mutex_)> lock(mutex_);

    OPENAUTO_LOG(info) << "[InputDevice] start.";
    eventHandler_ = &eventHandler;
    parent_.installEventFilter(this);
}

void InputDevice::stop()
{
    std::lock_guard<decltype(mutex_)> lock(mutex_);

    OPENAUTO_LOG(info) << "[InputDevice] stop.";
    parent_.removeEventFilter(this);
    eventHandler_ = nullptr;
}

bool InputDevice::eventFilter(QObject* obj, QEvent* event)
{
    std::lock_guard<decltype(mutex_)> lock(mutex_);

    if(eventHandler_ != nullptr)
    {
        if(event->type() == QEvent::KeyPress || event->type() == QEvent::KeyRelease)
        {
            QKeyEvent* key = static_cast<QKeyEvent*>(event);
            if(!key->isAutoRepeat())
            {
                return this->handleKeyEvent(event, key);
            }
        }
        else if(event->type() == QEvent::MouseButtonPress || event->type() == QEvent::MouseButtonRelease || event->type() == QEvent::MouseMove)
        {
            return this->handleTouchEvent(event);
        }
    }

    return QObject::eventFilter(obj, event);
}

bool InputDevice::handleKeyEvent(QEvent* event, QKeyEvent* key)
{
    auto eventType = event->type() == QEvent::KeyPress ? ButtonEventType::PRESS : ButtonEventType::RELEASE;
    aasdk::proto::enums::ButtonCode::Enum buttonCode;
    WheelDirection wheelDirection = WheelDirection::NONE;

    switch(key->key())
    {
    case Qt::Key_Return:
    case Qt::Key_Enter:
        buttonCode = aasdk::proto::enums::ButtonCode::ENTER;
        break;

    case Qt::Key_Left:
        buttonCode = aasdk::proto::enums::ButtonCode::LEFT;
        break;

    case Qt::Key_Right:
        buttonCode = aasdk::proto::enums::ButtonCode::RIGHT;
        break;

    case Qt::Key_Up:
        buttonCode = aasdk::proto::enums::ButtonCode::UP;
        break;

    case Qt::Key_Down:
        buttonCode = aasdk::proto::enums::ButtonCode::DOWN;
        break;

    case Qt::Key_Escape:
        buttonCode = aasdk::proto::enums::ButtonCode::BACK;
        break;

    case Qt::Key_Home:
        buttonCode = aasdk::proto::enums::ButtonCode::HOME;
        break;
        
    case Qt::Key_MediaPlay:
        buttonCode = aasdk::proto::enums::ButtonCode::PLAY;
        break;

    case Qt::Key_MediaPause:
        buttonCode = aasdk::proto::enums::ButtonCode::PAUSE;
        break;

    case Qt::Key_MediaPrevious:
        buttonCode = aasdk::proto::enums::ButtonCode::PREV;
        break;

    case Qt::Key_MediaNext:
        buttonCode = aasdk::proto::enums::ButtonCode::NEXT;
        break;

    case Qt::Key_Insert:
        buttonCode = aasdk::proto::enums::ButtonCode::MICROPHONE_1;
        break;

    case Qt::Key_PageUp:
        wheelDirection = WheelDirection::LEFT;
        eventType = ButtonEventType::NONE;
        buttonCode = aasdk::proto::enums::ButtonCode::SCROLL_WHEEL;
        break;

    case Qt::Key_PageDown:
        wheelDirection = WheelDirection::RIGHT;
        eventType = ButtonEventType::NONE;
        buttonCode = aasdk::proto::enums::ButtonCode::SCROLL_WHEEL;
        break;

    case Qt::Key_Space:
        buttonCode = aasdk::proto::enums::ButtonCode::SPACE;
        break;

    case Qt::Key_Backspace:
        buttonCode = aasdk::proto::enums::ButtonCode::BACKSPACE;
        break;

    case Qt::Key_Comma:
        buttonCode = aasdk::proto::enums::ButtonCode::COMMA;
        break;

    case Qt::Key_Period:
        buttonCode = aasdk::proto::enums::ButtonCode::DOT;
        break;

    case Qt::Key_Slash:
        buttonCode = aasdk::proto::enums::ButtonCode::SLASH;
        break;

    case Qt::Key_A:
    case Qt::Key_B:
    case Qt::Key_C:
    case Qt::Key_D:
    case Qt::Key_E:
    case Qt::Key_F:
    case Qt::Key_G:
    case Qt::Key_H:
    case Qt::Key_I:
    case Qt::Key_J:
    case Qt::Key_K:
    case Qt::Key_L:
    case Qt::Key_M:
    case Qt::Key_N:
    case Qt::Key_O:
    case Qt::Key_P:
    case Qt::Key_Q:
    case Qt::Key_R:
    case Qt::Key_S:
    case Qt::Key_T:
    case Qt::Key_U:
    case Qt::Key_V:
    case Qt::Key_W:
    case Qt::Key_X:
    case Qt::Key_Y:
    case Qt::Key_Z:
        buttonCode = static_cast<aasdk::proto::enums::ButtonCode::Enum>(aasdk::proto::enums::ButtonCode::A + (key->key() - Qt::Key_A));
        break;

    case Qt::Key_0:
    case Qt::Key_1:
    case Qt::Key_2:
    case Qt::Key_3:
    case Qt::Key_4:
    case Qt::Key_5:
    case Qt::Key_6:
    case Qt::Key_7:
    case Qt::Key_8:
    case Qt::Key_9:
        buttonCode = static_cast<aasdk::proto::enums::ButtonCode::Enum>(aasdk::proto::enums::ButtonCode::NUMBER_0 + (key->key() - Qt::Key_0));
        break;

    default:
        return true;
    }

    const auto& buttonCodes = this->getSupportedButtonCodes();
    if(std::find(buttonCodes.begin(), buttonCodes.end(), buttonCode) != buttonCodes.end())
    {
        if(buttonCode != aasdk::proto::enums::ButtonCode::SCROLL_WHEEL || event->type() == QEvent::KeyRelease)
        {
            eventHandler_->onButtonEvent({eventType, wheelDirection, buttonCode});
        }
    }

    return true;
}

bool InputDevice::handleTouchEvent(QEvent* event)
{
    if(!configuration_->getTouchscreenEnabled())
    {
        return true;
    }

    aasdk::proto::enums::TouchAction::Enum type;

    switch(event->type())
    {
    case QEvent::MouseButtonPress:
        type = aasdk::proto::enums::TouchAction::PRESS;
        break;
    case QEvent::MouseButtonRelease:
        type = aasdk::proto::enums::TouchAction::RELEASE;
        break;
    case QEvent::MouseMove:
        type = aasdk::proto::enums::TouchAction::DRAG;
        break;
    default:
        return true;
    };

    QMouseEvent* mouse = static_cast<QMouseEvent*>(event);
    if(event->type() == QEvent::MouseButtonRelease || mouse->buttons().testFlag(Qt::LeftButton))
    {
        const uint32_t x = (static_cast<float>(mouse->pos().x()) / touchscreenGeometry_.width()) * displayGeometry_.width();
        const uint32_t y = (static_cast<float>(mouse->pos().y()) / touchscreenGeometry_.height()) * displayGeometry_.height();
        eventHandler_->onTouchEvent({type, x, y, 0});
    }

    return true;
}

bool InputDevice::hasTouchscreen() const
{
    return configuration_->getTouchscreenEnabled();
}

QRect InputDevice::getTouchscreenGeometry() const
{
    return touchscreenGeometry_;
}

IInputDevice::ButtonCodes InputDevice::getSupportedButtonCodes() const
{
    return configuration_->getButtonCodes();
}

}
}
}
}
