import QtQuick
import QtQuick.Controls

import "qrc:/androidAuto"
import "qrc:/main"
import "qrc:/settings"

Window {
    width: 800
    height: 480
    visible: true
    //visibility: Window.FullScreen

    SwipeView {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: tabBar.top

        currentIndex: tabBar.currentIndex

        SettingsView {}

        MainView {}

        AndroidAutoView {}
    }

    TabBar {
        id: tabBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        currentIndex: 1

        TabButton {
            text: qsTr("Settings")
        }

        TabButton {
            text: qsTr("Main")
        }

        TabButton {
            text: qsTr("Android Auto")
        }
    }
}
