import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

ListView {
    id: listView
    orientation: ListView.Horizontal


    Item {
        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.verticalCenter: parent.verticalCenter
        height: 50
        width: height

//        visible: listView.currentIndex > 0

        Rectangle {
            anchors.fill: parent
            radius: height
            color: lefhtArrowMouseArea.containsMouse ? "#628DCF" : "#EEEEEE"
            opacity: 0.7
        }

        PlasmaCore.IconItem {
            anchors.centerIn: parent
            source: "arrow-left"
        }

        MouseArea {
            id: lefhtArrowMouseArea
            anchors.fill: parent
            onClicked: moveViewportBackward()
            hoverEnabled: true
        }
    }

    Item {
        anchors.right: parent.right
        anchors.rightMargin: 12
        anchors.verticalCenter: parent.verticalCenter

        height: 50
        width: height

//        visible: listView.currentIndex < listView.count

        Rectangle {
            anchors.fill: parent
            radius: height
            color: rightArrowMouseArea.containsMouse ? "#628DCF" : "#EEEEEE"
            opacity: 0.7
        }

        PlasmaCore.IconItem {
            anchors.centerIn: parent
            source: "arrow-right"
        }

        MouseArea {
            id: rightArrowMouseArea
            anchors.fill: parent
            onClicked: moveViewportForward()
            hoverEnabled: true
        }
    }

    function moveViewportForward() {
        flick(width * -2, 0)
    }
    function moveViewportBackward() {
        flick(width * 2, 0)
    }
}
