import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import "qrc:/commons"

ScreenElement {
    property int allElementsWidth: 2 * elementWidth + columnsMargin
    property int elementWidth: Math.min((width - (2 * margin) - columnsMargin) / 2, 500)
    property int elementHeight: guiStyle.mediumElementSize
    property int margin: 20
    property int columnsMargin: 60

    clip: true

    TabBar {
        id: bar
        width: allElementsWidth
        anchors.top: parent.top
        anchors.horizontalCenter: parent.horizontalCenter

        TabButton {
            text: qsTr("Android Auto")
        }

        TabButton {
            text: qsTr("Management")
        }
    }

    SwipeView {
        width: parent.width
        anchors.top: bar.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: margin

        clip: true
        currentIndex: bar.currentIndex

        AndroidAutoTab {
            enabled: bar.currentIndex == 0
        }

        ManagementTab {
            enabled: bar.currentIndex == 1
        }
    }
}
