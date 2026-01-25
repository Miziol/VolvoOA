import QtQuick
import QtQuick.Controls.Material

FocusScope {
    signal switchToSettings()
    signal switchToAndroidAuto()

    clip: true

    ToolButton {
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter

        focus: true
        text: qsTr("Settings")

        onClicked: switchToSettings()
    }

    Logo {
        anchors.fill: parent
        anchors.margins: 5
    }

    ToolButton {
        anchors.right: parent.right
        anchors.verticalCenter: parent.verticalCenter

        enabled: aaService.aaConnected

        text: qsTr("Android Auto")

        onClicked: switchToAndroidAuto()
    }

    Behavior on height {
        SmoothedAnimation {
            duration: animationTime
        }
    }
}