import QtQuick
import QtQuick.Shapes

Item {
    property int globalStrokeWidth: -1
    property color globalStrokeColor: "transparent"

    Shape {
        id: shape
        width: 100
        height: 50

        anchors.centerIn: parent

        ShapePath {
            strokeWidth: globalStrokeWidth
            strokeColor: globalStrokeColor
            fillGradient: logoGradient

            startX: 0; startY: 50
            PathLine {
                x: 12.5; y: 50
            }
            PathLine {
                x: 25; y: 0
            }
            PathLine {
                x: 12.5; y: 0
            }
        }

        ShapePath {
            strokeWidth: globalStrokeWidth
            strokeColor: globalStrokeColor
            fillGradient: logoGradient

            startX: 37.5; startY: 50
            PathLine {
                x: 50; y: 50
            }
            PathLine {
                x: 37.5; y: 0
            }
            PathLine {
                x: 25; y: 0
            }
        }

        ShapePath {
            strokeWidth: globalStrokeWidth
            strokeColor: globalStrokeColor
            fillGradient: logoGradient

            startX: 50; startY: 50
            PathLine {
                x: 62.5; y: 50
            }
            PathLine {
                x: 75; y: 0
            }
            PathLine {
                x: 62.5; y: 0
            }
        }

        ShapePath {
            strokeWidth: globalStrokeWidth
            strokeColor: globalStrokeColor
            fillGradient: logoGradient

            startX: 87.5; startY: 50
            PathLine {
                x: 100; y: 50
            }
            PathLine {
                x: 87.5; y: 0
            }
            PathLine {
                x: 75; y: 0
            }
        }

        ShapePath {
            strokeWidth: globalStrokeWidth
            strokeColor: globalStrokeColor
            fillGradient: logoGradient

            startX: 20; startY: 30
            PathLine {
                x: 30; y: 30
            }
            PathLine {
                x: 25; y: 10
            }
        }

        ShapePath {
            strokeWidth: globalStrokeWidth
            strokeColor: globalStrokeColor
            fillGradient: logoGradient

            startX: 70; startY: 30
            PathLine {
                x: 80; y: 30
            }
            PathLine {
                x: 75; y: 10
            }
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