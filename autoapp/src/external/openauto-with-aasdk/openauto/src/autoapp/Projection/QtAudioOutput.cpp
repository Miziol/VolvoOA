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
#include <QGuiApplication>
#include <QMediaDevices>
#include <f1x/openauto/autoapp/Projection/QtAudioOutput.hpp>

namespace f1x {
namespace openauto {
namespace autoapp {
namespace projection {

QtAudioOutput::QtAudioOutput(uint32_t channelCount, QAudioFormat::SampleFormat sampleFormat, uint32_t sampleRate)
    : playbackStarted_(false) {
    audioFormat_.setChannelCount(channelCount);
    audioFormat_.setSampleRate(sampleRate);
    audioFormat_.setSampleFormat(sampleFormat);

    this->moveToThread(QGuiApplication::instance()->thread());
    connect(this, &QtAudioOutput::startPlayback, this, &QtAudioOutput::onStartPlayback);
    connect(this, &QtAudioOutput::suspendPlayback, this, &QtAudioOutput::onSuspendPlayback);
    connect(this, &QtAudioOutput::stopPlayback, this, &QtAudioOutput::onStopPlayback);

    qDebug() << "[QtAudioOutput] create.";
    audioOutput_ = std::make_unique<QAudioSink>(QMediaDevices::defaultAudioOutput(), audioFormat_);
}

bool QtAudioOutput::open() {
    return audioBuffer_.open(QIODevice::ReadWrite);
}

void QtAudioOutput::write(aasdk::messenger::Timestamp::ValueType, const aasdk::common::DataConstBuffer &buffer) {
    audioBuffer_.write(reinterpret_cast<const char *>(buffer.cdata), buffer.size);
}

void QtAudioOutput::start() {
    emit startPlayback();
}

void QtAudioOutput::stop() {
    emit stopPlayback();
}

void QtAudioOutput::suspend() {
    emit suspendPlayback();
}

uint32_t QtAudioOutput::getSampleSize() const {
    switch (audioFormat_.sampleFormat()) {
        case QAudioFormat::Float:
        case QAudioFormat::Int32:
            return 32;
        case QAudioFormat::Int16:
            return 16;
        case QAudioFormat::UInt8:
            return 8;
        case QAudioFormat::Unknown:
        defualt:
            return 0;
    }
    return 0;
}

uint32_t QtAudioOutput::getChannelCount() const {
    return audioFormat_.channelCount();
}

uint32_t QtAudioOutput::getSampleRate() const {
    return audioFormat_.sampleRate();
}

void QtAudioOutput::onStartPlayback() {
    if (!playbackStarted_) {
        audioOutput_->start(&audioBuffer_);
        playbackStarted_ = true;
    } else {
        audioOutput_->resume();
    }
}

void QtAudioOutput::onSuspendPlayback() {
    audioOutput_->suspend();
}

void QtAudioOutput::onStopPlayback() {
    if (playbackStarted_) {
        audioOutput_->stop();
        playbackStarted_ = false;
    }
}

}  // namespace projection
}  // namespace autoapp
}  // namespace openauto
}  // namespace f1x
