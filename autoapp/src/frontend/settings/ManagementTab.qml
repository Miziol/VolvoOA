import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import "qrc:/commons"


FocusScope {
    implicitWidth: tabBar.implicitWidth
    implicitHeight: tabBar.implicitHeight

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
            text: qsTr("Navigation")
        }

        Switch {
            Layout.preferredWidth: elementWidth
            Layout.preferredHeight: elementHeight
            focus: true

            text: qsTr("Show cursor")

            checked: settingsManager.showCursor
            onClicked: settingsManager.showCursor = checked
        }

        Title {
            Layout.preferredWidth: allElementsWidth
            Layout.preferredHeight: elementHeight
            Layout.columnSpan: 2

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("Updates")
        }

        ComboBox {
            Layout.preferredWidth: elementWidth
            Layout.preferredHeight: elementHeight

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
}
