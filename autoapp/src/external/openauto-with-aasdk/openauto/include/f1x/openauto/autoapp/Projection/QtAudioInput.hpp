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

#include <QAudioFormat>
#include <QAudioSource>
#include <f1x/openauto/autoapp/Projection/IAudioInput.hpp>
#include <mutex>

namespace f1x {
namespace openauto {
namespace autoapp {
namespace projection {

class QtAudioInput : public QObject, public IAudioInput {
    Q_OBJECT
public:
    QtAudioInput(uint32_t channelCount, QAudioFormat::SampleFormat sampleFormat, uint32_t sampleRate);

    bool open() override;
    bool isActive() const override;
    void read(ReadPromise::Pointer promise) override;
    void start(StartPromise::Pointer promise) override;
    void stop() override;
    uint32_t getSampleSize() const override;
    uint32_t getChannelCount() const override;
    uint32_t getSampleRate() const override;

signals:
    void startRecording(StartPromise::Pointer promise);
    void stopRecording();

private slots:
    void onStartRecording(StartPromise::Pointer promise);
    void onStopRecording();
    void onReadyRead();

private:
    QAudioFormat audioFormat_;
    QIODevice *ioDevice_;
    std::unique_ptr<QAudioSource> audioInput_;
    ReadPromise::Pointer readPromise_;
    mutable std::mutex mutex_;

    static constexpr size_t cSampleSize = 2056;
};

}  // namespace projection
}  // namespace autoapp
}  // namespace openauto
}  // namespace f1x
