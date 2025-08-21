import QtQuick
import QtQuick.Controls.Material
import "qrc:/commons"

ScreenElement {
    ListView {
        model: usbService.usbDevices

        delegate: Button {
            text: modelData.deviceName

            onClicked: modelData.tryToStartAndroidAutoServer()
        }
    }
}
