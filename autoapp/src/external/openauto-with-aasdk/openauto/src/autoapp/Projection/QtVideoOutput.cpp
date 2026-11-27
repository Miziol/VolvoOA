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

#include <../../../../backend/settings/settingsManager.h>

#include <QDebug>
#include <QGuiApplication>
#include <QLoggingCategory>
#include <f1x/openauto/autoapp/Projection/QtVideoOutput.hpp>

// ponytail: this leaf helper has no per-object QLoggingCategory member like the backend classes
// (it lives in the vendored openauto tree), so it gets one file-scope category instead.
Q_LOGGING_CATEGORY(logVideoOutput, "QT VIDEO OUTPUT")

namespace f1x {
namespace openauto {
namespace autoapp {
namespace projection {
QtVideoOutput::QtVideoOutput(SettingsManager &configuration, QObject *new_qmlRootObject)
    : VideoOutput(configuration), qmlRootObject(new_qmlRootObject) {
    this->moveToThread(QGuiApplication::instance()->thread());
    connect(this, &QtVideoOutput::startPlayback, this, &QtVideoOutput::onStartPlayback, Qt::QueuedConnection);
    connect(this, &QtVideoOutput::stopPlayback, this, &QtVideoOutput::onStopPlayback, Qt::QueuedConnection);

    auto *videoOutput = qmlRootObject->findChild<QObject *>("aaVideoOutput");
    mediaPlayer.setVideoOutput(videoOutput);
    connect(&mediaPlayer, &QMediaPlayer::errorOccurred, this, &QtVideoOutput::onMediaError);
}

bool QtVideoOutput::open() {
    return videoBuffer_.open(QIODevice::ReadWrite);
}

bool QtVideoOutput::init() {
    emit startPlayback();
    return true;
}

void QtVideoOutput::stop() {
    emit stopPlayback();
}

void QtVideoOutput::write(uint64_t, const aasdk::common::DataConstBuffer &buffer) {
    videoBuffer_.write(reinterpret_cast<const char *>(buffer.cdata), buffer.size);
}

void QtVideoOutput::onStartPlayback() {
    mediaPlayer.setSourceDevice(&videoBuffer_, QString("video/h264"));
    mediaPlayer.play();
}

void QtVideoOutput::onStopPlayback() {
    mediaPlayer.stop();
}

void QtVideoOutput::onMediaError(QMediaPlayer::Error error, const QString &errorString) {
    qCCritical(logVideoOutput) << "media error:" << error << errorString;
}
}  // namespace projection
}  // namespace autoapp
}  // namespace openauto
}  // namespace f1x
