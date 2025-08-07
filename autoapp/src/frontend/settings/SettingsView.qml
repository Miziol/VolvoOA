import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import "qrc:/commons"

ScreenElement {
    property int margin: 40
    property int elementHeight: guiStyle.mediumElementSize

    GridLayout {
        anchors.fill: parent
        anchors.margins: margin

        columns: 2
        columnSpacing: margin

        Title {
            height: elementHeight
            Layout.columnSpan: 2
            text: qsTr("General settings")
        }

        Switch {
            width: 400
            height: elementHeight
            text: qsTr("Show clock in AA")

            checked: settingsManager.showClock
            onClicked: settingsManager.showClock = checked
        }

        Switch {
            width: 400
            height: elementHeight
            text: qsTr("Left hand drive")

            checked: settingsManager.leftHandDrive
            onClicked: settingsManager.leftHandDrive = checked
        }

        Title {
            height: elementHeight
            Layout.columnSpan: 2
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

            RoundButton{
                height: elementHeight
                ButtonGroup.group: resolutionGroup
                checkable: true
                checked: settingsManager.videoResolution === 2

                text: qsTr("720")
                onClicked: {
                    settingsManager.videoResolution = 2
                }
            }

            RoundButton{
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

        Title {
            height: elementHeight
            Layout.columnSpan: 2
            text: qsTr("Navigation")
        }

        Switch {
            width: 400
            height: elementHeight
            text: qsTr("Show cursor")

            checked: settingsManager.showCursor
            onClicked: settingsManager.showCursor = checked
        }
    }
}
