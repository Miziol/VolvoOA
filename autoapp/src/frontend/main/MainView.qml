import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import "qrc:/commons"

ScreenElement {
    RowLayout {
        anchors.fill: parent

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            model: usbService.usbDevices
            enabled: model.length > 0 && !aaService.aaBusy
            focus: true

            delegate: Button {
                width: parent.width

                text: modelData.displayName

                onClicked: aaService.connectDevice(modelData)
            }

            EmptyListIcon {
                width: 50
                anchors.centerIn: parent
                visible: parent.model.length == 0
                tipText: qsTr("Connect device")
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Switch {
                Layout.fillWidth: true

                text: qsTr("Discoverable")
                checked: bluetoothService.discoverable
                onClicked: bluetoothService.discoverable = checked
            }

            ListView {
                Layout.fillWidth: true
                Layout.fillHeight: true

                model: bluetoothService.bluetoothDevices
                enabled: model.length > 0 && !aaService.aaBusy

                delegate: Button {
                    width: parent.width

                    text: modelData.displayName + (modelData.connected ? "  ✓" : "")

                    onClicked: aaService.connectDevice(modelData)
                }

                EmptyListIcon {
                    width: 50
                    anchors.centerIn: parent
                    visible: parent.model.length == 0
                    tipText: qsTr("Pair phone")
                }
            }
        }
    }
}
