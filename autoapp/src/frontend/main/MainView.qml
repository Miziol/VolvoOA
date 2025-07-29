import QtQuick
import QtQuick.Controls
import "qrc:/commons"

ScreenElement {
    ListView {
        model: usbService.usbDevices

        delegate: Button {
            text: modelData.deviceName

            onClicked: modelData.tryToStartAndroidAutoServer()
        }
    }

    color: "green"
}
