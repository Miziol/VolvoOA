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
        enabled: viewIndex != 2

        onSwitchToSettings: viewIndex ? viewIndex = 0 : viewIndex = 1
        onSwitchToAndroidAuto: viewIndex = 2

        KeyNavigation.tab: viewIndex == 2 ? null : viewIndex ? mainView : settingsView
    }

    SettingsView {
        id: settingsView
        x: (0 - viewIndex) * parent.width
        width: parent.width
        anchors.top: appBar.bottom
        anchors.bottom: parent.bottom

        enabled: x == 0

        KeyNavigation.backtab: appBar
        KeyNavigation.tab: appBar
    }

    MainView {
        id: mainView
        x: (1 - viewIndex) * parent.width
        width: parent.width
        anchors.top: appBar.bottom
        anchors.bottom: parent.bottom

        enabled: x == 0
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

        enabled: x == 0
    }

    FocusIndicator {
        id: focusIndicator
        visible: viewIndex != 2
    }

    onActiveFocusItemChanged: {
        if (activeFocusItem != null) {
            focusIndicator.width = activeFocusItem.width
            focusIndicator.height = activeFocusItem.height

            const activeFocusItemLocation = activeFocusItem.mapToItem(null, 0, 0)
            focusIndicator.x = activeFocusItemLocation.x
            focusIndicator.y = activeFocusItemLocation.y
        }
    }

    Connections {
        target: aaService

        function onFocusOnAA(focus) {
            if (focus) {
                viewIndex = 2
                androidAutoView.forceActiveFocus()
            } else {
                viewIndex = 1
            }
        }
    }
}
