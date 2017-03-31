import QtQuick 2.7
 import QtGraphicalEffects 1.0
import org.kde.plasma.core 2.0 as PlasmaCore

Item {
    id: root
    signal clicked;
    Rectangle {
        anchors.fill: parent
        radius: height
        color: mousearea.containsMouse ? "#628DCF" : "#EEEEEE"
        opacity: 0.7
    }
    
    PlasmaCore.IconItem {
        id: icon
        anchors.centerIn: parent
        source: "go-previous"
        opacity: mousearea.containsMouse ? 1 : 0.7

        Behavior on opacity { PropertyAnimation{} }
    }

    MouseArea {
        id: mousearea
        anchors.fill: parent
        hoverEnabled: true
    }

    Component.onCompleted: {
        mousearea.clicked.connect(clicked)
    }
}
