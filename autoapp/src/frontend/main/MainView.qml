import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import Autoapp 1.0  // AaDeviceEntry enum for the USB/Wireless type icon
import "qrc:/commons"

ScreenElement {
    ColumnLayout {
        anchors.fill: parent

        Switch {
            Layout.fillWidth: true

            text: qsTr("Discoverable")
            checked: bluetoothService.discoverable
            onClicked: bluetoothService.discoverable = checked
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            model: usbService.usbDevices.concat(bluetoothService.bluetoothDevices)
            enabled: model.length > 0 && !aaService.aaBusy
            focus: true

            delegate: Button {
                width: parent.width

                text: modelData.displayName + (modelData.connected ? "  ✓" : "")

                onClicked: aaService.connectDevice(modelData)
            }

            EmptyListIcon {
                width: 50
                anchors.centerIn: parent
                visible: parent.model.length == 0
                tipText: qsTr("Connect or pair device")
            }
        }
    }
}
