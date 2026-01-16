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
#include <f1x/openauto/autoapp/Projection/QtAudioInput.hpp>

namespace f1x {
namespace openauto {
namespace autoapp {
namespace projection {
QtAudioInput::QtAudioInput(uint32_t channelCount,
                           QAudioFormat::SampleFormat sampleFormat,
                           uint32_t sampleRate)
    : ioDevice_(nullptr) {
    qRegisterMetaType<IAudioInput::StartPromise::Pointer>("StartPromise::Pointer");

    audioFormat_.setChannelCount(channelCount);
    audioFormat_.setSampleRate(sampleRate);
    audioFormat_.setSampleFormat(sampleFormat);

    this->moveToThread(QGuiApplication::instance()->thread());
    connect(this, &QtAudioInput::startRecording, this, &QtAudioInput::onStartRecording,
            Qt::QueuedConnection);
    connect(this, &QtAudioInput::stopRecording, this, &QtAudioInput::onStopRecording,
            Qt::QueuedConnection);

    qDebug() << "[AudioInput] create.";
    audioInput_ = (std::make_unique<QAudioSource>(QMediaDevices::defaultAudioInput(), audioFormat_));
}

bool QtAudioInput::open() {
    std::lock_guard<decltype(mutex_)> lock(mutex_);

    return ioDevice_ == nullptr;
}

bool QtAudioInput::isActive() const {
    std::lock_guard<decltype(mutex_)> lock(mutex_);

    return ioDevice_ != nullptr;
}

void QtAudioInput::read(ReadPromise::Pointer promise) {
    std::lock_guard<decltype(mutex_)> lock(mutex_);

    if (ioDevice_ == nullptr) {
        promise->reject();
    } else if (readPromise_ != nullptr) {
        promise->reject();
    } else {
        readPromise_ = std::move(promise);
    }
}

void QtAudioInput::start(StartPromise::Pointer promise) {
    emit startRecording(std::move(promise));
}

void QtAudioInput::stop() {
    emit stopRecording();
}

uint32_t QtAudioInput::getSampleSize() const {
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

uint32_t QtAudioInput::getChannelCount() const {
    return audioFormat_.channelCount();
}

uint32_t QtAudioInput::getSampleRate() const {
    return audioFormat_.sampleRate();
}

void QtAudioInput::onStartRecording(StartPromise::Pointer promise) {
    std::lock_guard<decltype(mutex_)> lock(mutex_);

    ioDevice_ = audioInput_->start();

    if (ioDevice_ != nullptr) {
        connect(ioDevice_, &QIODevice::readyRead, this, &QtAudioInput::onReadyRead,
                Qt::QueuedConnection);
        promise->resolve();
    } else {
        promise->reject();
    }
}

void QtAudioInput::onStopRecording() {
    std::lock_guard<decltype(mutex_)> lock(mutex_);

    if (readPromise_ != nullptr) {
        readPromise_->reject();
        readPromise_.reset();
    }

    if (ioDevice_ != nullptr) {
        ioDevice_->reset();
        ioDevice_->disconnect();
        ioDevice_ = nullptr;
    }

    audioInput_->stop();
}

void QtAudioInput::onReadyRead() {
    std::lock_guard<decltype(mutex_)> lock(mutex_);

    if (readPromise_ == nullptr) {
        return;
    }

    aasdk::common::Data data(cSampleSize, 0);
    aasdk::common::DataBuffer buffer(data);
    auto readSize = ioDevice_->read(reinterpret_cast<char *>(buffer.data), buffer.size);

    if (readSize != -1) {
        data.resize(readSize);
        readPromise_->resolve(std::move(data));
        readPromise_.reset();
    } else {
        readPromise_->reject();
        readPromise_.reset();
    }
}
} // namespace projection
} // namespace autoapp
} // namespace openauto
} // namespace f1x
