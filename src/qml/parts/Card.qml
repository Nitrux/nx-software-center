import QtQuick 2.0
import QtGraphicalEffects 1.0

Item {
    property alias border: background.border
    property alias color: background.color

    Rectangle {
        id: background
        width: parent.width
        height: parent.height

        radius: 2
    }

    DropShadow {
        anchors.fill: background
        radius: 6
        samples: 13
        color: "#AFAFB1"
        verticalOffset: 0
        spread: 0
        source: background
    }
}
