import QtQuick
import QtQuick.Controls.Material

Rectangle {
    border.width: activeFocusItem ? guiStyle.detailSize : 0
    border.color: Material.accentColor
    radius: 100
    color: "transparent"

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
}