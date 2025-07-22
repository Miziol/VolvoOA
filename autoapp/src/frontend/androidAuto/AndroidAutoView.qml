import QtQuick
import QtMultimedia
import "qrc:/commons"

ScreenElement {
    color: "blue"

    VideoOutput {
        objectName: "aaVideoOutput"
        anchors.fill: parent
    }
}
