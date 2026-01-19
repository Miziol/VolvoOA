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

    ComboBox {
        model: arduinoService.arduinosList
        currentIndex: arduinoService.currentIndex

        onCurrentIndexChanged: arduinoService.currentIndex = currentIndex
    }

    Row {
        RoundButton {
            height: elementHeight
            enabled: arduinoService.currentIndex != -1

            text: qsTr("Update Arduino Firmware")

            onClicked: arduinoService.updateSelectedArduinoFirmware()
        }

        BusyIndicator {
            height: elementHeight
            running: arduinoService.arduinoUpdater.running
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
}
