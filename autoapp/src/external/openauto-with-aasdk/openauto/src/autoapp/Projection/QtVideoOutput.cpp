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
#include <f1x/openauto/autoapp/Projection/QtVideoOutput.hpp>

namespace f1x {
namespace openauto {
namespace autoapp {
namespace projection {

QtVideoOutput::QtVideoOutput(SettingsManager &configuration, QObject *new_qmlRootObject)
    : VideoOutput(configuration), qmlRootObject(new_qmlRootObject) {
    this->moveToThread(QGuiApplication::instance()->thread());
    connect(this, &QtVideoOutput::startPlayback, this, &QtVideoOutput::onStartPlayback, Qt::QueuedConnection);
    connect(this, &QtVideoOutput::stopPlayback, this, &QtVideoOutput::onStopPlayback, Qt::QueuedConnection);

    qDebug() << "[QtVideoOutput] create.";
    mediaPlayer.setVideoOutput(qmlRootObject->findChild<QObject *>("aaVideoOutput"));
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

}  // namespace projection
}  // namespace autoapp
}  // namespace openauto
}  // namespace f1x
