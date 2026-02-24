import QtQuick

FocusScope {
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