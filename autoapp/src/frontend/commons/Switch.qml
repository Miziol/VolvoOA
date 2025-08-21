import QtQuick
import QtQuick.Controls.Material

Switch {
    property int paddingFromBoarder: 5

    indicator.anchors.right: indicator.parent.right
    indicator.anchors.rightMargin: paddingFromBoarder
    contentItem.anchors.left: contentItem.parent.left

    font.pixelSize: guiStyle.labelTextSize

    Component.onCompleted: {
        contentItem.rightPadding = contentItem.leftPadding
        contentItem.leftPadding = paddingFromBoarder
    }
}