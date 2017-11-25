import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents


import org.nx.softwarecenter 1.0


Item {
    property alias message: messageText.text
    property alias iconName: icon.source
    property alias showBusyIndicator: busyModelIndicator.visible
    objectName: "placeHolderView"

    PlasmaComponents.BusyIndicator {
        id: busyModelIndicator
//        visible: storeSnapsModel.busy
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -60
    }

    PlasmaComponents.Label {
        id: messageText
        anchors.top: busyModelIndicator.bottom
        anchors.topMargin: 32
        anchors.horizontalCenter: parent.horizontalCenter
        width: 400
        height: 100
        
//        text: storeSnapsModel.errorMessage
//              == "" ? storeSnapsModel.statusMessage : storeSnapsModel.errorMessage
        
        fontSizeMode: Text.Fit
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.WordWrap
        minimumPixelSize: 10
        font.pixelSize: 20
    }
    

    
    PlasmaCore.IconItem {
        id: icon
        anchors.centerIn: parent
        anchors.verticalCenterOffset: -60

        width: 64
        height: 64
        visible: !busyModelIndicator.visible
    }
}
