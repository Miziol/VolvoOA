import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import "qrc:/commons"

FocusScope {
    implicitWidth: tabBar.width
    implicitHeight: tabBar.height

    GridLayout {
        id: tabBar
        columns: 2
        columnSpacing: columnsMargin

        Title {
            Layout.preferredWidth: allElementsWidth
            Layout.preferredHeight: elementHeight
            Layout.columnSpan: 2

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("General settings")
        }

        Switch {
            id: aaClockSwitch
            Layout.preferredWidth: elementWidth
            Layout.preferredHeight: elementHeight
            focus: true

            text: qsTr("Show clock in AA")

            checked: settingsManager.showClock
            onClicked: settingsManager.showClock = checked
        }

        Switch {
            Layout.preferredWidth: elementWidth
            Layout.preferredHeight: elementHeight
            text: qsTr("Left hand drive")

            checked: settingsManager.leftHandDrive
            onClicked: settingsManager.leftHandDrive = checked
        }

        Title {
            Layout.preferredWidth: allElementsWidth
            Layout.preferredHeight: elementHeight
            Layout.columnSpan: 2

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("Video")
        }

        Row {
            ButtonGroup {
                id: resolutionGroup
            }

            Label {
                height: elementHeight
                text: qsTr("Resolution")
            }

            RoundButton {
                height: elementHeight
                ButtonGroup.group: resolutionGroup
                checkable: true
                checked: settingsManager.videoResolution === 0

                text: qsTr("-")
                onClicked: {
                    settingsManager.videoResolution = 0
                }
            }

            RoundButton {
                height: elementHeight
                ButtonGroup.group: resolutionGroup
                checkable: true
                checked: settingsManager.videoResolution === 1

                text: qsTr("480")
                onClicked: {
                    settingsManager.videoResolution = 1
                }
            }

            RoundButton {
                height: elementHeight
                ButtonGroup.group: resolutionGroup
                checkable: true
                checked: settingsManager.videoResolution === 2

                text: qsTr("720")
                onClicked: {
                    settingsManager.videoResolution = 2
                }
            }

            RoundButton {
                height: elementHeight
                ButtonGroup.group: resolutionGroup
                checkable: true
                checked: settingsManager.videoResolution === 3

                text: qsTr("1080")
                onClicked: {
                    settingsManager.videoResolution = 3
                }
            }
        }

        Row {
            ButtonGroup {
                id: fpsGroup
            }

            Label {
                height: elementHeight
                text: qsTr("FPS")
            }

            RoundButton {
                height: elementHeight
                ButtonGroup.group: fpsGroup
                checkable: true
                checked: settingsManager.videoFPS === 0

                text: qsTr("-")
                onClicked: {
                    settingsManager.videoFPS = 0
                }
            }

            RoundButton {
                height: elementHeight
                ButtonGroup.group: fpsGroup
                checkable: true
                checked: settingsManager.videoFPS === 1

                text: qsTr("30")
                onClicked: {
                    settingsManager.videoFPS = 1
                }
            }

            RoundButton {
                height: elementHeight
                ButtonGroup.group: fpsGroup
                checkable: true
                checked: settingsManager.videoFPS === 2

                text: qsTr("60")
                onClicked: {
                    settingsManager.videoFPS = 2
                }
            }
        }

        Row {
            Label {
                height: elementHeight
                text: qsTr("DPI") + " " + settingsManager.screenDPI
            }

            Slider {
                height: elementHeight
                from: 120
                to: 240
                value: settingsManager.screenDPI
                onValueChanged: {
                    if (value != settingsManager.screenDPI) {
                        settingsManager.screenDPI = value
                    }
                }
            }
        }
    }
}

