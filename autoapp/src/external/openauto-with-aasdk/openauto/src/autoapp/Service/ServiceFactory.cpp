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

#include <QGuiApplication>
#include <QScreen>
#include <f1x/aasdk/Channel/AV/MediaAudioServiceChannel.hpp>
#include <f1x/aasdk/Channel/AV/SpeechAudioServiceChannel.hpp>
#include <f1x/aasdk/Channel/AV/SystemAudioServiceChannel.hpp>
#include <f1x/openauto/autoapp/Projection/DummyBluetoothDevice.hpp>
#include <f1x/openauto/autoapp/Projection/InputDevice.hpp>
#include <f1x/openauto/autoapp/Projection/LocalBluetoothDevice.hpp>
#include <f1x/openauto/autoapp/Projection/QtAudioInput.hpp>
#include <f1x/openauto/autoapp/Projection/QtAudioOutput.hpp>
#include <f1x/openauto/autoapp/Projection/QtVideoOutput.hpp>
#include <f1x/openauto/autoapp/Projection/RemoteBluetoothDevice.hpp>
#include <f1x/openauto/autoapp/Service/AudioInputService.hpp>
#include <f1x/openauto/autoapp/Service/BluetoothService.hpp>
#include <f1x/openauto/autoapp/Service/InputService.hpp>
#include <f1x/openauto/autoapp/Service/MediaAudioService.hpp>
#include <f1x/openauto/autoapp/Service/SensorService.hpp>
#include <f1x/openauto/autoapp/Service/ServiceFactory.hpp>
#include <f1x/openauto/autoapp/Service/SpeechAudioService.hpp>
#include <f1x/openauto/autoapp/Service/SystemAudioService.hpp>
#include <f1x/openauto/autoapp/Service/VideoService.hpp>

#include "../../../../../../backend/settings/settingsManager.h"

namespace f1x {
namespace openauto {
namespace autoapp {
namespace service {
ServiceFactory::ServiceFactory(boost::asio::io_service &ioService,
                               SettingsManager &configuration,
                               QObject *new_qmlRootObject)
    : ioService_(ioService), configuration_(configuration), qmlRootObject(new_qmlRootObject) {}

ServiceList ServiceFactory::create(IAndroidAutoEntityEventHandler *handler,
                                   aasdk::messenger::IMessenger::Pointer messenger) {
    ServiceList serviceList;

    projection::IAudioInput::Pointer audioInput(
        new projection::QtAudioInput(1, QAudioFormat::Int16, 16000),
        std::bind(&QObject::deleteLater, std::placeholders::_1));
    serviceList.emplace_back(
        std::make_shared<AudioInputService>(ioService_, messenger, std::move(audioInput)));
    this->createAudioServices(serviceList, messenger);
    serviceList.emplace_back(std::make_shared<SensorService>(ioService_, messenger));
    serviceList.emplace_back(this->createVideoService(handler, messenger));
    serviceList.emplace_back(this->createBluetoothService(messenger));
    serviceList.emplace_back(this->createInputService(messenger));

    return serviceList;
}

IService::Pointer ServiceFactory::createVideoService(IAndroidAutoEntityEventHandler *handler,
                                                     aasdk::messenger::IMessenger::Pointer messenger) {
    projection::IVideoOutput::Pointer videoOutput(
        new projection::QtVideoOutput(configuration_, qmlRootObject),
        std::bind(&QObject::deleteLater, std::placeholders::_1));

    return std::make_shared<VideoService>(handler, ioService_, messenger, std::move(videoOutput));
}

IService::Pointer ServiceFactory::createBluetoothService(
    aasdk::messenger::IMessenger::Pointer messenger) {
    projection::IBluetoothDevice::Pointer bluetoothDevice;
    switch (configuration_.getBluetoothAdapterType()) {
        case configuration::BluetoothAdapterType::LOCAL:
            bluetoothDevice = projection::IBluetoothDevice::Pointer(
                new projection::LocalBluetoothDevice(),
                std::bind(&QObject::deleteLater, std::placeholders::_1));
            break;

        case configuration::BluetoothAdapterType::REMOTE:
            bluetoothDevice = std::make_shared<projection::RemoteBluetoothDevice>(
                configuration_.getBluetoothRemoteAdapterAddress().toString().toStdString());
            break;

        default:
            bluetoothDevice = std::make_shared<projection::DummyBluetoothDevice>();
            break;
    }

    return std::make_shared<BluetoothService>(ioService_, messenger, std::move(bluetoothDevice));
}

IService::Pointer ServiceFactory::createInputService(aasdk::messenger::IMessenger::Pointer messenger) {
    QObject *videoOutput = qmlRootObject->findChild<QObject *>("aaVideoOutput");
    QRect inputGeometry = videoOutput->property("contentRect").toRect();

    QRect videoGeometry;
    switch (configuration_.getVideoResolution()) {
        case aasdk::proto::enums::VideoResolution::_720p:
            videoGeometry = QRect(0, 0, 1280, 720);
            break;

        case aasdk::proto::enums::VideoResolution::_1080p:
            videoGeometry = QRect(0, 0, 1920, 1080);
            break;

        default:
            videoGeometry = QRect(0, 0, 800, 480);
            break;
    }

    projection::IInputDevice::Pointer inputDevice(std::make_shared<projection::InputDevice>(
        configuration_, videoOutput, std::move(inputGeometry), std::move(videoGeometry)));
    return std::make_shared<InputService>(ioService_, messenger, std::move(inputDevice));
}

void ServiceFactory::createAudioServices(ServiceList &serviceList,
                                         aasdk::messenger::IMessenger::Pointer messenger) {
    if (true) {
        // TODO configuration_.musicAudioChannelEnabled()
        auto mediaAudioOutput =
            projection::IAudioOutput::Pointer(
                new projection::QtAudioOutput(2, QAudioFormat::Int16, 48000),
                std::bind(&QObject::deleteLater, std::placeholders::_1));

        serviceList.emplace_back(
            std::make_shared<MediaAudioService>(ioService_, messenger, std::move(mediaAudioOutput)));
    }

    if (true) {
        // TODO configuration_.speechAudioChannelEnabled()
        auto speechAudioOutput =
            projection::IAudioOutput::Pointer(
                new projection::QtAudioOutput(1, QAudioFormat::Int16, 16000),
                std::bind(&QObject::deleteLater, std::placeholders::_1));

        serviceList.emplace_back(
            std::make_shared<SpeechAudioService>(ioService_, messenger, std::move(speechAudioOutput)));
    }

    auto systemAudioOutput =
        projection::IAudioOutput::Pointer(new projection::QtAudioOutput(1, QAudioFormat::Int16, 16000),
                                          std::bind(&QObject::deleteLater, std::placeholders::_1));

    serviceList.emplace_back(
        std::make_shared<SystemAudioService>(ioService_, messenger, std::move(systemAudioOutput)));
}
} // namespace service
} // namespace autoapp
} // namespace openauto
} // namespace f1x
