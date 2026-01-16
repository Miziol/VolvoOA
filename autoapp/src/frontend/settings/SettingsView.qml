import QtQuick
import QtQuick.Controls.Material
import QtQuick.Layouts
import "qrc:/commons"

ScreenElement {
    property int margin: 20
    property int elementHeight: guiStyle.mediumElementSize

    Item {
        anchors.fill: parent
        anchors.margins: margin

        TabBar {
            id: bar
            width: parent.width
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right

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
            anchors.bottom: parent.bottom
            anchors.topMargin: margin

            clip: true
            currentIndex: bar.currentIndex

            AndroidAutoTab {
            }

            ManagementTab {
            }
        }
    }
}
