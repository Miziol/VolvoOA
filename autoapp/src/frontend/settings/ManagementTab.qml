import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import "qrc:/commons"


GridLayout {
    columns: 2
    columnSpacing: margin

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

    Title {
        height: elementHeight
        Layout.columnSpan: 2
        text: qsTr("Updates")
    }

    Row {
        RoundButton {
            height: elementHeight
            enabled: !systemUpdater.running

            text: qsTr("Update Arduino Firmware")

            // onClicked: core.updateSystem() // TODO
        }

        BusyIndicator {
            height: elementHeight
            // running: systemUpdater.running // TODO
        }
    }

    Row {
        spacing: margin

        RoundButton {
            height: elementHeight
            enabled: !appUpdater.running

            text: qsTr("Update App")

            onClicked: core.updateApp()
        }

        ProgressBar {
            height: elementHeight
            visible: appUpdater.running
            value: appUpdater.step / appUpdater.steps
        }
    }

    Row {
        RoundButton {
            height: elementHeight
            enabled: !systemUpdater.running

            text: qsTr("Update System")

            onClicked: core.updateSystem()
        }

        BusyIndicator {
            height: elementHeight
            running: systemUpdater.running
        }
    }

    Rectangle {
        Layout.columnSpan: 2

        color: "black"

        Flickable {
            Column {
                Repeater {
                    model: ["TET", "TEST"] // arduinoUpdater.output

                    Text {
                        text: modelData
                    }
                }
            }
        }
    }
}
