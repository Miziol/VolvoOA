import QtQuick
import QtQuick.Controls.Material
import "qrc:/commons"

ScreenElement {
    ListView {
        anchors.fill: parent
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
        }
    }
}
