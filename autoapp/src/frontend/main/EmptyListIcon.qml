import QtQuick

Column {
    id: root
    property color color: "grey"

    height: 2 * width
    spacing: parent.height / 9

    Rectangle {
        width: parent.width
        height: parent.height * 7/9
        anchors.horizontalCenter: parent.horizontalCenter
        radius: 15

        color: "transparent"
        border.color: parent.color
        border.width: width/10

        Rectangle {
            width: 2 * parent.border.width
            height: width
            radius: width/2
            color: root.color

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: parent.border.width + guiStyle.detailSize
        }
    }

    Text {
        height: parent.height * 1/9
        anchors.horizontalCenter: parent.horizontalCenter

        color: root.color
        font.pixelSize: guiStyle.titleTextSize
        text: qsTr("Connect device")
    }
}
