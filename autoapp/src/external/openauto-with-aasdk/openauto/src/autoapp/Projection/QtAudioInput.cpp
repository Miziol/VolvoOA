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
#include <f1x/openauto/autoapp/Projection/QtAudioInput.hpp>

namespace f1x {
namespace openauto {
namespace autoapp {
namespace projection {

QtAudioInput::QtAudioInput(uint32_t channelCount, uint32_t sampleSize, uint32_t sampleRate) : ioDevice_(nullptr) {
    qRegisterMetaType<IAudioInput::StartPromise::Pointer>("StartPromise::Pointer");
    /*
        audioFormat_.setChannelCount(channelCount);
        audioFormat_.setSampleRate(sampleRate);
        audioFormat_.setSampleSize(sampleSize);
        audioFormat_.setCodec("audio/pcm");
        audioFormat_.setByteOrder(QAudioFormat::LittleEndian);
        audioFormat_.setSampleType(QAudioFormat::SignedInt);
    */ // TODO migrate Qt6
    this->moveToThread(QGuiApplication::instance()->thread());
    connect(this, &QtAudioInput::startRecording, this, &QtAudioInput::onStartRecording, Qt::QueuedConnection);
    connect(this, &QtAudioInput::stopRecording, this, &QtAudioInput::onStopRecording, Qt::QueuedConnection);

    createAudioInput();  // TODO remove ?
    // QMetaObject::invokeMethod(this, "createAudioInput", Qt::BlockingQueuedConnection);
}

void QtAudioInput::createAudioInput() {
    qDebug() << "[AudioInput] create.";
    // audioInput_ = (std::make_unique<QAudioInput>(QAudioDeviceInfo::defaultInputDevice(), audioFormat_)); // TODO
    // migrate Qt6
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
    return 2056;
    // return audioFormat_.sampleSize(); // TODO migrate Qt6
}

uint32_t QtAudioInput::getChannelCount() const {
    return audioFormat_.channelCount();
}

uint32_t QtAudioInput::getSampleRate() const {
    return audioFormat_.sampleRate();
}

void QtAudioInput::onStartRecording(StartPromise::Pointer promise) {
    std::lock_guard<decltype(mutex_)> lock(mutex_);

    // ioDevice_ = audioInput_->start(); // TODO migrate Qt6

    if (ioDevice_ != nullptr) {
        // connect(ioDevice_, &QIODevice::readyRead, this, &QtAudioInput::onReadyRead, Qt::QueuedConnection); // TODO
        // migrate Qt6
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
        /*
        ioDevice_->reset();
        ioDevice_->disconnect();
        */ // TODO migrate Qt6
        ioDevice_ = nullptr;
    }

    // audioInput_->stop(); // TODO migrate Qt6
}

void QtAudioInput::onReadyRead() {
    std::lock_guard<decltype(mutex_)> lock(mutex_);

    if (readPromise_ == nullptr) {
        return;
    }

    aasdk::common::Data data(cSampleSize, 0);
    aasdk::common::DataBuffer buffer(data);
    /* auto readSize = ioDevice_->read(reinterpret_cast<char*>(buffer.data), buffer.size);

    if(readSize != -1)
    {
        data.resize(readSize);
        readPromise_->resolve(std::move(data));
        readPromise_.reset();
    }
    else
    {
        readPromise_->reject();
        readPromise_.reset();
    }
    */ // TODO migrate Qt6
}

}  // namespace projection
}  // namespace autoapp
}  // namespace openauto
}  // namespace f1x
