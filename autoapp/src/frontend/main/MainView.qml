import QtQuick
import QtQuick.Controls
import "qrc:/commons"

ScreenElement {
    ListView {
        model: aaService.aaDevices

        delegate: Button {
            text: modelData.name

            onClicked: modelData.start()
        }
    }

    color: "green"
}
