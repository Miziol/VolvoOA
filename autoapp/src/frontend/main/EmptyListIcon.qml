import QtQuick

Item {
    width: 200
    height: 200

    // Główna obudowa (odpowiednik <rect>)
    Rectangle {
        x: 60; y: 30
        width: 80
        height: 140
        radius: 10 // rx, ry

        color: "transparent" // fill="none"
        border.color: "grey" // stroke="grey"
        border.width: 15     // stroke-width="15"

        // Kropka w środku (odpowiednik <circle>)
        // Pozycjonujemy względem rodzica (obudowy)
        Rectangle {
            width: 30  // r * 2
            height: 30
            radius: 15 // połowa szerokootności daje koło
            color: "grey"

            // Wyśrodkowanie w poziomie i dół (zgodnie z cy="140" w SVG)
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 15 // Dostosowane do cy=140
        }
    }
}
