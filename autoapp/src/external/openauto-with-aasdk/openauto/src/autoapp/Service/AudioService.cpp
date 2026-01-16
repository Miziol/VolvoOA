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

#include <QDebug>
#include <f1x/openauto/autoapp/Service/AudioService.hpp>

namespace f1x {
namespace openauto {
namespace autoapp {
namespace service {
AudioService::AudioService(boost::asio::io_service &ioService,
                           aasdk::channel::av::IAudioServiceChannel::Pointer channel,
                           projection::IAudioOutput::Pointer audioOutput)
    : strand_(ioService), channel_(std::move(channel)), audioOutput_(std::move(audioOutput)),
      session_(-1) {}

void AudioService::start() {
    strand_.dispatch([this, self = this->shared_from_this()]() {
        qInfo() << "[AudioService] start, channel: "
            << QString::fromStdString(aasdk::messenger::channelIdToString(channel_->getId()));
        channel_->receive(this->shared_from_this());
    });
}

void AudioService::stop() {
    strand_.dispatch([this, self = this->shared_from_this()]() {
        qInfo() << "[AudioService] stop, channel: "
            << QString::fromStdString(aasdk::messenger::channelIdToString(channel_->getId()));
        audioOutput_->stop();
    });
}

void AudioService::fillFeatures(aasdk::proto::messages::ServiceDiscoveryResponse &response) {
    qInfo() << "[AudioService] fill features, channel: "
        << QString::fromStdString(aasdk::messenger::channelIdToString(channel_->getId()));

    auto *channelDescriptor = response.add_channels();
    channelDescriptor->set_channel_id(static_cast<uint32_t>(channel_->getId()));

    auto *audioChannel = channelDescriptor->mutable_av_channel();
    audioChannel->set_stream_type(aasdk::proto::enums::AVStreamType::AUDIO);

    switch (channel_->getId()) {
        case aasdk::messenger::ChannelId::SYSTEM_AUDIO:
            audioChannel->set_audio_type(aasdk::proto::enums::AudioType::SYSTEM);
            break;

        case aasdk::messenger::ChannelId::MEDIA_AUDIO:
            audioChannel->set_audio_type(aasdk::proto::enums::AudioType::MEDIA);
            break;

        case aasdk::messenger::ChannelId::SPEECH_AUDIO:
            audioChannel->set_audio_type(aasdk::proto::enums::AudioType::SPEECH);
            break;
        default:
            break;
    }

    audioChannel->set_available_while_in_call(true);

    auto *audioConfig = audioChannel->add_audio_configs();
    audioConfig->set_sample_rate(audioOutput_->getSampleRate());
    audioConfig->set_bit_depth(audioOutput_->getSampleSize());
    audioConfig->set_channel_count(audioOutput_->getChannelCount());
}

void AudioService::onChannelOpenRequest(const aasdk::proto::messages::ChannelOpenRequest &request) {
    qInfo() << "[AudioService] open request"
        << ", channel: " << QString::fromStdString(
            aasdk::messenger::channelIdToString(channel_->getId()))
        << ", priority: " << request.priority();

    qDebug() << "[AudioService] channel: "
        << QString::fromStdString(aasdk::messenger::channelIdToString(channel_->getId()))
        << " audio output sample rate: " << audioOutput_->getSampleRate()
        << ", sample size: " << audioOutput_->getSampleSize()
        << ", channel count: " << audioOutput_->getChannelCount();

    const aasdk::proto::enums::Status::Enum status =
        audioOutput_->open() ? aasdk::proto::enums::Status::OK : aasdk::proto::enums::Status::FAIL;
    qInfo() << "[AudioService] open status: " << status
        << ", channel: " << QString::fromStdString(
            aasdk::messenger::channelIdToString(channel_->getId()));

    aasdk::proto::messages::ChannelOpenResponse response;
    response.set_status(status);

    auto promise = aasdk::channel::SendPromise::defer(strand_);
    promise->then([]() {}, std::bind(&AudioService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendChannelOpenResponse(response, std::move(promise));
    channel_->receive(this->shared_from_this());
}

void AudioService::onAVChannelSetupRequest(const aasdk::proto::messages::AVChannelSetupRequest &request) {
    qInfo() << "[AudioService] setup request"
        << ", channel: " << QString::fromStdString(
            aasdk::messenger::channelIdToString(channel_->getId()))
        << ", config index: " << request.config_index();
    const aasdk::proto::enums::AVChannelSetupStatus::Enum status =
        aasdk::proto::enums::AVChannelSetupStatus::OK;
    qInfo() << "[AudioService] setup status: " << status
        << ", channel: " << QString::fromStdString(
            aasdk::messenger::channelIdToString(channel_->getId()));

    aasdk::proto::messages::AVChannelSetupResponse response;
    response.set_media_status(status);
    response.set_max_unacked(1);
    response.add_configs(0);

    auto promise = aasdk::channel::SendPromise::defer(strand_);
    promise->then([]() {}, std::bind(&AudioService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendAVChannelSetupResponse(response, std::move(promise));
    channel_->receive(this->shared_from_this());
}

void AudioService::onAVChannelStartIndication(
    const aasdk::proto::messages::AVChannelStartIndication &indication) {
    qInfo() << "[AudioService] start indication"
        << ", channel: " << QString::fromStdString(
            aasdk::messenger::channelIdToString(channel_->getId()))
        << ", session: " << indication.session();
    session_ = indication.session();
    audioOutput_->start();
    channel_->receive(this->shared_from_this());
}

void AudioService::onAVChannelStopIndication(
    const aasdk::proto::messages::AVChannelStopIndication &indication) {
    qInfo() << "[AudioService] stop indication"
        << ", channel: " << QString::fromStdString(
            aasdk::messenger::channelIdToString(channel_->getId()))
        << ", session: " << session_;
    session_ = -1;
    audioOutput_->suspend();
    channel_->receive(this->shared_from_this());
}

void AudioService::onAVMediaWithTimestampIndication(aasdk::messenger::Timestamp::ValueType timestamp,
                                                    const aasdk::common::DataConstBuffer &buffer) {
    audioOutput_->write(timestamp, buffer);
    aasdk::proto::messages::AVMediaAckIndication indication;
    indication.set_session(session_);
    indication.set_value(1);

    auto promise = aasdk::channel::SendPromise::defer(strand_);
    promise->then([]() {}, std::bind(&AudioService::onChannelError, this->shared_from_this(), std::placeholders::_1));
    channel_->sendAVMediaAckIndication(indication, std::move(promise));
    channel_->receive(this->shared_from_this());
}

void AudioService::onAVMediaIndication(const aasdk::common::DataConstBuffer &buffer) {
    this->onAVMediaWithTimestampIndication(0, buffer);
}

void AudioService::onChannelError(const aasdk::error::Error &e) {
    qCritical() << "[AudioService] channel error: " << e.what()
        << ", channel: " << QString::fromStdString(
            aasdk::messenger::channelIdToString(channel_->getId()));
}
} // namespace service
} // namespace autoapp
} // namespace openauto
} // namespace f1x
