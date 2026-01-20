import QtQuick
import QtQuick.Shapes

Item {
    Shape {
        id: shape
        width: 100
        height: 50
        anchors.centerIn: parent

        layer.enabled: true
        layer.samples: 8
        smooth: true
        antialiasing: true

        ShapePath {
            strokeWidth: 0
            strokeColor: "transparent"
            fillColor: "white" // TODO

            startX: 0; startY: 50
            PathLine { x: 12.5; y: 50 }
            PathLine { x: 25; y: 0 }
            PathLine { x: 37.5; y: 50 }
            PathLine { x: 50; y: 50 }
            PathLine { x: 37.5; y: 0 }
            PathLine { x: 12.5; y: 0 }

            PathMove { x: 20; y: 30 }
            PathLine { x: 30; y: 30 }
            PathLine { x: 25; y: 10 }

            PathMove { x: 50; y: 50 }
            PathLine { x: 62.5; y: 50 }
            PathLine { x: 75; y: 0 }
            PathLine { x: 87.5; y: 50 }
            PathLine { x: 100; y: 50 }
            PathLine { x: 87.5; y: 0 }
            PathLine { x: 62.5; y: 0 }

            PathMove { x: 70; y: 30 }
            PathLine { x: 80; y: 30 }
            PathLine { x: 75; y: 10 }
        }
    }

    LinearGradient {
        id: logoGradient
        x1: 0; y1: 0
        x2: 100; y2: 50

        GradientStop {
            position: 0; color: "white"
        }
        GradientStop {
            position: 1; color: "white"
        }
    }
}