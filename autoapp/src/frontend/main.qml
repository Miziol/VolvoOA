import QtQuick
import QtQuick.Controls.Material

import "qrc:/androidAuto"
import "qrc:/main"
import "qrc:/settings"
import "qrc:/window"

Window {
    property int viewIndex: 1

    property int barHeight: 40
    property int animationTime: 200

    width: 800
    height: 480
    visible: true
    visibility: Window.FullScreen
    color: Material.background

    Material.theme: Material.Dark
    Material.accent: Material.Green

    MainBar {
        id: appBar
        z: 1
        width: parent.width
        height: viewIndex != 2 ? barHeight : 0

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        onSwitchToSettings: viewIndex ? viewIndex = 0 : viewIndex = 1
        onSwitchToAndroidAuto: viewIndex = 2

        KeyNavigation.backtab: null
        KeyNavigation.tab: viewIndex == 2 ? null : viewIndex ? mainView : settingsView
    }

    SettingsView {
        id: settingsView
        x: (0 - viewIndex) * parent.width
        anchors.top: appBar.bottom
        anchors.bottom: parent.bottom

        width: parent.width

        KeyNavigation.backtab: appBar
        KeyNavigation.tab: appBar
    }

    MainView {
        id: mainView
        x: (1 - viewIndex) * parent.width
        anchors.top: appBar.bottom
        anchors.bottom: parent.bottom

        width: parent.width
        focus: true

        KeyNavigation.backtab: appBar
        KeyNavigation.tab: appBar
    }

    AndroidAutoView {
        id: androidAutoView
        x: (2 - viewIndex) * parent.width

        width: parent.width
        height: parent.height

        anchors.verticalCenter: parent.verticalCenter
    }

    FocusIndicator {
        id: focusIndicator
    }

    onActiveFocusItemChanged: {
        focusIndicator.width = activeFocusItem.width
        focusIndicator.height = activeFocusItem.height

        const activeFocusItemLocation = activeFocusItem.mapToGlobal(0, 0)
        focusIndicator.x = activeFocusItemLocation.x
        focusIndicator.y = activeFocusItemLocation.y
    }

    Connections {
        target: aaService

        function onFocusOnAA(focus) {
            if (focus) {
                viewIndex = 2
            } else {
                viewIndex = 1
            }
        }
    }
}
