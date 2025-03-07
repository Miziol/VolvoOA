import QtQuick
import QtQuick.Controls

Rectangle {
    ListView {
        model: aaService.aaDevices

        delegate: Button {
            text: modelData.name

            onClicked: modelData.start()
        }
    }

    color: "green"
}
