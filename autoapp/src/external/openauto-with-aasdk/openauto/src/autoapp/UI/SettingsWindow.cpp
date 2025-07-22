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

#include <QMessageBox>
#include <f1x/openauto/autoapp/UI/SettingsWindow.hpp>
#include "ui_settingswindow.h"

namespace f1x
{
namespace openauto
{
namespace autoapp
{
namespace ui
{

SettingsWindow::SettingsWindow(configuration::IConfiguration::Pointer configuration, QWidget *parent)
    : QWidget(parent)
    , ui_(new Ui::SettingsWindow)
    , configuration_(std::move(configuration))
{
    ui_->setupUi(this);
    connect(ui_->pushButtonCancel, &QPushButton::clicked, this, &SettingsWindow::close);
    connect(ui_->pushButtonSave, &QPushButton::clicked, this, &SettingsWindow::onSave);
    connect(ui_->horizontalSliderScreenDPI, &QSlider::valueChanged, this, &SettingsWindow::onUpdateScreenDPI);
    connect(ui_->radioButtonUseExternalBluetoothAdapter, &QRadioButton::clicked, [&](bool checked) { ui_->lineEditExternalBluetoothAdapterAddress->setEnabled(checked); });
    connect(ui_->radioButtonDisableBluetooth, &QRadioButton::clicked, [&]() { ui_->lineEditExternalBluetoothAdapterAddress->setEnabled(false); });
    connect(ui_->radioButtonUseLocalBluetoothAdapter, &QRadioButton::clicked, [&]() { ui_->lineEditExternalBluetoothAdapterAddress->setEnabled(false); });
    connect(ui_->pushButtonClearSelection, &QPushButton::clicked, std::bind(&SettingsWindow::setButtonCheckBoxes, this, false));
    connect(ui_->pushButtonSelectAll, &QPushButton::clicked, std::bind(&SettingsWindow::setButtonCheckBoxes, this, true));
    connect(ui_->pushButtonResetToDefaults, &QPushButton::clicked, this, &SettingsWindow::onResetToDefaults);
}

SettingsWindow::~SettingsWindow()
{
    delete ui_;
}

void SettingsWindow::onSave()
{
    configuration_->setHandednessOfTrafficType(ui_->radioButtonLeftHandDrive->isChecked() ? configuration::HandednessOfTrafficType::LEFT_HAND_DRIVE : configuration::HandednessOfTrafficType::RIGHT_HAND_DRIVE);
    configuration_->showClock(ui_->checkBoxShowClock->isChecked());
    configuration_->setVideoFPS(ui_->radioButton30FPS->isChecked() ? aasdk::proto::enums::VideoFPS::_30 : aasdk::proto::enums::VideoFPS::_60);

    if(ui_->radioButton480p->isChecked())
    {
        configuration_->setVideoResolution(aasdk::proto::enums::VideoResolution::_480p);
    }
    else if(ui_->radioButton720p->isChecked())
    {
        configuration_->setVideoResolution(aasdk::proto::enums::VideoResolution::_720p);
    }
    else if(ui_->radioButton1080p->isChecked())
    {
        configuration_->setVideoResolution(aasdk::proto::enums::VideoResolution::_1080p);
    }

    configuration_->setScreenDPI(static_cast<size_t>(ui_->horizontalSliderScreenDPI->value()));
    configuration_->setOMXLayerIndex(ui_->spinBoxOmxLayerIndex->value());

    QRect videoMargins(0, 0, ui_->spinBoxVideoMarginWidth->value(), ui_->spinBoxVideoMarginHeight->value());
    configuration_->setVideoMargins(std::move(videoMargins));

    configuration_->setTouchscreenEnabled(ui_->checkBoxEnableTouchscreen->isChecked());
    this->saveButtonCheckBoxes();

    if(ui_->radioButtonDisableBluetooth->isChecked())
    {
        configuration_->setBluetoothAdapterType(configuration::BluetoothAdapterType::NONE);
    }
    else if(ui_->radioButtonUseLocalBluetoothAdapter->isChecked())
    {
        configuration_->setBluetoothAdapterType(configuration::BluetoothAdapterType::LOCAL);
    }
    else if(ui_->radioButtonUseExternalBluetoothAdapter->isChecked())
    {
        configuration_->setBluetoothAdapterType(configuration::BluetoothAdapterType::REMOTE);
    }

    configuration_->setBluetoothRemoteAdapterAddress(ui_->lineEditExternalBluetoothAdapterAddress->text().toStdString());

    configuration_->setMusicAudioChannelEnabled(ui_->checkBoxMusicAudioChannel->isChecked());
    configuration_->setSpeechAudioChannelEnabled(ui_->checkBoxSpeechAudioChannel->isChecked());
    configuration_->setAudioOutputBackendType(ui_->radioButtonRtAudio->isChecked() ? configuration::AudioOutputBackendType::RTAUDIO : configuration::AudioOutputBackendType::QT);

    configuration_->save();
    this->close();
}

void SettingsWindow::onResetToDefaults()
{
    QMessageBox confirmationMessage(QMessageBox::Question, "Confirmation", "Are you sure you want to reset settings?", QMessageBox::Yes | QMessageBox::Cancel);
    confirmationMessage.setWindowFlags(Qt::WindowStaysOnTopHint);
    if(confirmationMessage.exec() == QMessageBox::Yes)
    {
        configuration_->reset();
        this->load();
    }
}

void SettingsWindow::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);
    this->load();
}

void SettingsWindow::load()
{
    ui_->radioButtonLeftHandDrive->setChecked(configuration_->getHandednessOfTrafficType() == configuration::HandednessOfTrafficType::LEFT_HAND_DRIVE);
    ui_->radioButtonRightHandDrive->setChecked(configuration_->getHandednessOfTrafficType() == configuration::HandednessOfTrafficType::RIGHT_HAND_DRIVE);
    ui_->checkBoxShowClock->setChecked(configuration_->showClock());

    ui_->radioButton30FPS->setChecked(configuration_->getVideoFPS() == aasdk::proto::enums::VideoFPS::_30);
    ui_->radioButton60FPS->setChecked(configuration_->getVideoFPS() == aasdk::proto::enums::VideoFPS::_60);

    ui_->radioButton480p->setChecked(configuration_->getVideoResolution() == aasdk::proto::enums::VideoResolution::_480p);
    ui_->radioButton720p->setChecked(configuration_->getVideoResolution() == aasdk::proto::enums::VideoResolution::_720p);
    ui_->radioButton1080p->setChecked(configuration_->getVideoResolution() == aasdk::proto::enums::VideoResolution::_1080p);
    ui_->horizontalSliderScreenDPI->setValue(static_cast<int>(configuration_->getScreenDPI()));
    ui_->spinBoxOmxLayerIndex->setValue(configuration_->getOMXLayerIndex());

    const auto& videoMargins = configuration_->getVideoMargins();
    ui_->spinBoxVideoMarginWidth->setValue(videoMargins.width());
    ui_->spinBoxVideoMarginHeight->setValue(videoMargins.height());

    ui_->checkBoxEnableTouchscreen->setChecked(configuration_->getTouchscreenEnabled());
    this->loadButtonCheckBoxes();

    ui_->radioButtonDisableBluetooth->setChecked(configuration_->getBluetoothAdapterType() == configuration::BluetoothAdapterType::NONE);
    ui_->radioButtonUseLocalBluetoothAdapter->setChecked(configuration_->getBluetoothAdapterType() == configuration::BluetoothAdapterType::LOCAL);
    ui_->radioButtonUseExternalBluetoothAdapter->setChecked(configuration_->getBluetoothAdapterType() == configuration::BluetoothAdapterType::REMOTE);
    ui_->lineEditExternalBluetoothAdapterAddress->setEnabled(configuration_->getBluetoothAdapterType() == configuration::BluetoothAdapterType::REMOTE);
    ui_->lineEditExternalBluetoothAdapterAddress->setText(QString::fromStdString(configuration_->getBluetoothRemoteAdapterAddress()));

    ui_->checkBoxMusicAudioChannel->setChecked(configuration_->musicAudioChannelEnabled());
    ui_->checkBoxSpeechAudioChannel->setChecked(configuration_->speechAudioChannelEnabled());

    const auto& audioOutputBackendType = configuration_->getAudioOutputBackendType();
    ui_->radioButtonRtAudio->setChecked(audioOutputBackendType == configuration::AudioOutputBackendType::RTAUDIO);
    ui_->radioButtonQtAudio->setChecked(audioOutputBackendType == configuration::AudioOutputBackendType::QT);
}

void SettingsWindow::loadButtonCheckBoxes()
{
    const auto& buttonCodes = configuration_->getButtonCodes();
    ui_->checkBoxEnterButton->setChecked(std::find(buttonCodes.begin(), buttonCodes.end(), aasdk::proto::enums::ButtonCode::ENTER) != buttonCodes.end());
    ui_->checkBoxArrowsButton->setChecked(std::find(buttonCodes.begin(), buttonCodes.end(), aasdk::proto::enums::ButtonCode::UP) != buttonCodes.end());
    ui_->checkBoxScrollWheelButton->setChecked(std::find(buttonCodes.begin(), buttonCodes.end(), aasdk::proto::enums::ButtonCode::SCROLL_WHEEL) != buttonCodes.end());
    ui_->checkBoxBackButton->setChecked(std::find(buttonCodes.begin(), buttonCodes.end(), aasdk::proto::enums::ButtonCode::BACK) != buttonCodes.end());
    ui_->checkBoxHomeButton->setChecked(std::find(buttonCodes.begin(), buttonCodes.end(), aasdk::proto::enums::ButtonCode::HOME) != buttonCodes.end());
    ui_->checkBoxMediaButton->setChecked(std::find(buttonCodes.begin(), buttonCodes.end(), aasdk::proto::enums::ButtonCode::PLAY) != buttonCodes.end());
    ui_->checkBoxVoiceCommandButton->setChecked(std::find(buttonCodes.begin(), buttonCodes.end(), aasdk::proto::enums::ButtonCode::MICROPHONE_1) != buttonCodes.end());
    ui_->checkBoxCharsButton->setChecked(std::find(buttonCodes.begin(), buttonCodes.end(), aasdk::proto::enums::ButtonCode::BACKSPACE) != buttonCodes.end());
    ui_->checkBoxLettersButton->setChecked(std::find(buttonCodes.begin(), buttonCodes.end(), aasdk::proto::enums::ButtonCode::A) != buttonCodes.end());
    ui_->checkBoxNumbersButton->setChecked(std::find(buttonCodes.begin(), buttonCodes.end(), aasdk::proto::enums::ButtonCode::NUMBER_0) != buttonCodes.end());
}

void SettingsWindow::setButtonCheckBoxes(bool value)
{
    ui_->checkBoxEnterButton->setChecked(value);
    ui_->checkBoxArrowsButton->setChecked(value);
    ui_->checkBoxScrollWheelButton->setChecked(value);
    ui_->checkBoxBackButton->setChecked(value);
    ui_->checkBoxHomeButton->setChecked(value);
    ui_->checkBoxMediaButton->setChecked(value);
    ui_->checkBoxVoiceCommandButton->setChecked(value);
    ui_->checkBoxCharsButton->setChecked(value);
    ui_->checkBoxLettersButton->setChecked(value);
    ui_->checkBoxNumbersButton->setChecked(value);
}

void SettingsWindow::saveButtonCheckBoxes()
{
    configuration::IConfiguration::ButtonCodes buttonCodes;

    if (ui_->checkBoxEnterButton->isChecked()) {
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::ENTER);
    }

    if (ui_->checkBoxArrowsButton->isChecked()) {
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::UP);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::LEFT);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::RIGHT);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::DOWN);
    }

    if (ui_->checkBoxScrollWheelButton->isChecked()) {
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::SCROLL_WHEEL);
    }

    if (ui_->checkBoxBackButton->isChecked()) {
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::BACK);
    }

    if (ui_->checkBoxHomeButton->isChecked()) {
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::HOME);
    }

    if (ui_->checkBoxMediaButton->isChecked()) {
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::PLAY);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::PAUSE);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::TOGGLE_PLAY);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::NEXT);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::PREV);
    }

    if (ui_->checkBoxVoiceCommandButton->isChecked()) {
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::MICROPHONE_1);
    }

    if (ui_->checkBoxCharsButton->isChecked()) {
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::BACKSPACE);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::SPACE);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::COMMA);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::DOT);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::SLASH);
    }

    if (ui_->checkBoxLettersButton->isChecked()) {
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::A);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::B);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::C);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::D);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::E);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::F);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::G);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::H);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::I);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::J);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::K);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::L);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::M);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::N);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::O);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::P);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::Q);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::R);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::S);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::T);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::U);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::V);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::W);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::X);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::Y);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::Z);
    }

    if (ui_->checkBoxNumbersButton->isChecked()) {
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::NUMBER_0);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::NUMBER_1);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::NUMBER_2);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::NUMBER_3);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::NUMBER_4);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::NUMBER_5);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::NUMBER_6);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::NUMBER_7);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::NUMBER_8);
        buttonCodes.push_back(aasdk::proto::enums::ButtonCode::NUMBER_9);
    }

    configuration_->setButtonCodes(buttonCodes);
}

void SettingsWindow::onUpdateScreenDPI(int value)
{
    ui_->labelScreenDPIValue->setText(QString::number(value));
}

void SettingsWindow::onShowBindings()
{
    const QString message = QString("Enter -> [Enter] \n")
                            + QString("Left -> [Left] \n")
                            + QString("Right -> [Right] \n")
                            + QString("Up -> [Up] \n")
                            + QString("Down -> [Down] \n")
                            + QString("Back -> [Esc] \n")
                            + QString("Home -> [H] \n")
                            + QString("Phone -> [P] \n")
                            + QString("Call end -> [O] \n")
                            + QString("Play -> [X] \n")
                            + QString("Pause -> [C] \n")
                            + QString("Previous track -> [V]/[Media Previous] \n")
                            + QString("Next track -> [N]/[Media Next] \n")
                            + QString("Toggle play -> [B]/[Media Play] \n")
                            + QString("Voice command -> [M] \n")
                            + QString("Wheel left -> [1] \n")
                            + QString("Wheel right -> [2]");

    QMessageBox confirmationMessage(QMessageBox::Information, "Information", message, QMessageBox::Ok);
    confirmationMessage.setWindowFlags(Qt::WindowStaysOnTopHint);
    confirmationMessage.exec();
}

}
}
}
}
