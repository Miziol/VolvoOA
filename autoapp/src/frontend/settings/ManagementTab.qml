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

    Button {
        text: qsTr("Update Arduino Firmware")

        onClicked: arduinoService.updateSelectedArduinoFirmware()
    }

    Button {
        text: qsTr("Update Arduino Firmware")

        onClicked: arduinoService.updateSelectedArduinoFirmware()
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
