import QtQuick

Rectangle {
    signal elementSelected

    property bool asButton: false
    property int animationTime: 200

    Behavior on x {
        SmoothedAnimation {
            duration: animationTime
        }
    }

    Behavior on y {
        SmoothedAnimation {
            duration: animationTime
        }
    }

    Behavior on width {
        SmoothedAnimation {
            duration: animationTime
        }
    }

    Behavior on height {
        SmoothedAnimation {
            duration: animationTime
        }
    }

    MouseArea {
        enabled: asButton
        anchors.fill: parent

        onClicked: elementSelected()
    }
}