import QtQuick
import QtQuick.Controls.Material

Item {
    signal switchToSettings()
    signal switchToAndroidAuto()

    clip: true

    ToolButton {
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter

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

        text: qsTr("Android Auto")

        onClicked: switchToAndroidAuto()
    }

    Behavior on height {
        SmoothedAnimation {
            duration: animationTime
        }
    }
}