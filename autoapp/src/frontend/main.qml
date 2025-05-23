import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material

import "qrc:/androidAuto"
import "qrc:/main"
import "qrc:/settings"

Window {
    property bool selectView: false
    property int miniatureSize: (mainView.width - ((mainView.children.length + 1) * miniatureSpacing)) / mainView.children.length
    property int miniatureRadius: 10
    property int miniatureSpacing: 20

    width: 800
    height: 480
    visible: true
    //visibility: Window.FullScreen
    color: Material.background

    Material.theme: Material.Dark
    Material.accent: Material.Green

    Rectangle {
        id: mainView
        z: 5
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: tabBar.top

        color: "transparent"

        SettingsView {
            x: selectView ?
                miniatureSpacing + (Array.prototype.slice.call(parent.children).indexOf(this) * (miniatureSize + miniatureSpacing)) :
                (Array.prototype.slice.call(parent.children).indexOf(this) - tabBar.currentIndex) * parent.width

            width: selectView ? miniatureSize : parent.width
            height: selectView ? miniatureSize : parent.height

            anchors.verticalCenter: parent.verticalCenter

            radius: selectView ? miniatureRadius : 0
            asButton: selectView

            onElementSelected: {
                tabBar.currentIndex = Array.prototype.slice.call(parent.children).indexOf(this)
                selectView = false
            }
        }

        MainView {
            x: selectView ?
                miniatureSpacing + (Array.prototype.slice.call(parent.children).indexOf(this) * (miniatureSize + miniatureSpacing)) :
                (Array.prototype.slice.call(parent.children).indexOf(this) - tabBar.currentIndex) * parent.width

            width: selectView ? miniatureSize : parent.width
            height: selectView ? miniatureSize : parent.height

            anchors.verticalCenter: parent.verticalCenter

            focus: true
            radius: selectView ? miniatureRadius : 0
            asButton: selectView

            onElementSelected: {
                tabBar.currentIndex = Array.prototype.slice.call(parent.children).indexOf(this)
                selectView = false
            }
        }

        AndroidAutoView {
            x: selectView ?
                miniatureSpacing + (Array.prototype.slice.call(parent.children).indexOf(this) * (miniatureSize + miniatureSpacing)) :
                (Array.prototype.slice.call(parent.children).indexOf(this) - tabBar.currentIndex) * parent.width

            width: selectView ? miniatureSize : parent.width
            height: selectView ? miniatureSize : parent.height

            anchors.verticalCenter: parent.verticalCenter

            radius: selectView ? miniatureRadius : 0
            asButton: selectView

            onElementSelected: {
                tabBar.currentIndex = Array.prototype.slice.call(parent.children).indexOf(this)
                selectView = false
            }
        }

        Keys.onPressed: (event) => {
            keyboardKeyPressed(event)
        }
    }

    Rectangle {
        id: marker
        x: miniatureSpacing + (tabBar.currentIndex * (miniatureSize + miniatureSpacing)) - (miniatureSpacing / 4)
        z: 0
        width: miniatureSize + (miniatureSpacing / 2)
        height: miniatureSize + (miniatureSpacing / 2)
        anchors.verticalCenter: mainView.verticalCenter

        visible: selectView
        radius: miniatureRadius
        color: Material.accent

        Behavior on x {
            SmoothedAnimation {
                duration: 200
            }
        }
    }

    TabBar {
        id: tabBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        currentIndex: 1

        TabButton {
            icon.source: "qrc:/icons/settings.svg"

            onClicked: selectView = !selectView
        }

        TabButton {
            icon.source: "qrc:/icons/dashboard.svg"

            onClicked: selectView = !selectView
        }

        TabButton {
            icon.source: "qrc:/icons/phone.svg"

            onClicked: selectView = !selectView
        }

        onCurrentIndexChanged: {
            marker.x = miniatureSpacing + (currentIndex * (miniatureSize + miniatureSpacing)) - (miniatureSpacing / 2)
        }

        Keys.onPressed: (event) => {
            keyboardKeyPressed(event)
        }
    }

    function keyboardKeyPressed(event) {
        if (event.key == Qt.Key_Home) {
            selectView = true
        } else if (event.key == Qt.Key_PageUp) {
            tabBar.currentIndex = tabBar.currentIndex > 0 ? tabBar.currentIndex - 1 : mainView.children.length - 1
        } else if (event.key == Qt.Key_PageDown) {
            tabBar.currentIndex = tabBar.currentIndex == mainView.children.length - 1 ? 0 : tabBar.currentIndex + 1
        } else if (event.key == Qt.Key_Enter || event.key == Qt.Key_Return) {
            selectView = false
        }
    }
}
