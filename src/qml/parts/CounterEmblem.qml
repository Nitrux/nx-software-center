import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

Item {
    property alias value: tasksCount.text

    visible: tasksCount.text != "0"
    opacity: tasksCount.text != "0" ? 1 : 0
    
    SequentialAnimation on opacity {
        id: glowAnimation
        NumberAnimation { to: 0.3; duration: 250 }
        NumberAnimation { to: 1; duration: 250 }
    }
    
    Rectangle {
        anchors.fill: parent
        anchors.margins: 1
        radius: width
        
        color: "#EC407A"
        border.color: "#D81B60"
        border.width: 1
    }
    
    PlasmaComponents.Label {
        id: tasksCount
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -0.8
        anchors.horizontalCenterOffset: 0.8
        
        color: "white"
        font.bold: true
        font.pointSize: 9
        onTextChanged: glowAnimation.start()
    }
}
