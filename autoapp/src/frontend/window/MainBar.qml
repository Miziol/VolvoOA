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

    Image {
        anchors.fill: parent
        anchors.margins: 5
        fillMode: Image.PreserveAspectFit

        source: "qrc:/icons/logo.svg"
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