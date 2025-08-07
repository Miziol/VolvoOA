import QtQuick
import QtQuick.Controls.Material

Switch {
    indicator.anchors.right: indicator.parent.right
    contentItem.anchors.left: contentItem.parent.left

    font.pixelSize: guiStyle.labelTextSize

    Component.onCompleted: {
        contentItem.rightPadding = contentItem.leftPadding
        contentItem.leftPadding = 0
    }
}