import QtQuick
import QtMultimedia
import "qrc:/commons"

ScreenElement {
    color: "blue"

    VideoOutput {
        id: aaVideoOutput

        signal keyEvent(key: int, pressed: bool)
        signal mouseEvent(x: real, y: real, type: string)

        objectName: "aaVideoOutput"
        anchors.fill: parent

        fillMode: Image.Stretch

        Keys.onPressed: (event) => {
            keyEvent(event.key, true)
            event.accepted = true
        }
        Keys.onReleased: (event) => {
            keyEvent(event.key, false)
            event.accepted = true
        }

        MouseArea {
            anchors.fill: parent

            onPressed: (event) => {
                aaVideoOutput.mouseEvent(event.x, event.y, "PRESSED")
            }
            onReleased: (event) => {
                aaVideoOutput.mouseEvent(event.x, event.y, "RELEASED")
            }
            onPositionChanged: (event) => {
                aaVideoOutput.mouseEvent(event.x, event.y, "DRAG")
            }
        }
    }

    onActiveFocusChanged: {
        if (activeFocus) {
            aaVideoOutput.forceActiveFocus()
        }
    }
}
