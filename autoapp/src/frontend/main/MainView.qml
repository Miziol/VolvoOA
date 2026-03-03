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
            enabled: model.length > 0
            focus: true

            delegate: Button {
                width: parent.width

                text: modelData.deviceName

                onClicked: modelData.tryToStartAndroidAutoServer()
            }

            EmptyListIcon {
                width: 50
                anchors.centerIn: parent
                visible: parent.model.length == 0
                tipText: qsTr("Connect device")
            }
        }

        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            model: networkService.networkDevices
            enabled: model.length > 0

            delegate: Button {
                width: parent.width

                text: modelData.hostName() + " (" + modelData.addresses()[0] + ")"

                onClicked: aaService.addNetworkDevice(modelData.addresses()[0])
            }

            EmptyListIcon {
                width: 50
                anchors.centerIn: parent
                visible: parent.model.length == 0 // TODO add scanning notification
                tipText: qsTr("Power on AA server")
            }
        }
    }
}
