import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

ListView {
    id: listView
    orientation: ListView.Horizontal


    ArrowLeft {
        anchors.left: parent.left
        anchors.leftMargin: 12
        anchors.verticalCenter: parent.verticalCenter
        height: 50
        width: height

//        visible: listView.currentIndex > 0

        onClicked: moveViewportBackward()
    }

    ArrowRight {
        anchors.right: parent.right
        anchors.rightMargin: 12
        anchors.verticalCenter: parent.verticalCenter

        height: 50
        width: height

//        visible: listView.currentIndex < listView.count

        onClicked: moveViewportForward()
    }

    function moveViewportForward() {
        flick(width * -2, 0)
    }
    function moveViewportBackward() {
        flick(width * 2, 0)
    }
}
