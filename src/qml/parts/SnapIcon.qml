import QtQuick 2.0
import org.kde.plasma.core 2.0 as PlasmaCore

Rectangle {
    height: 222
    width: 222
    property alias source: iconRemote.source

    color: "transparent"
    clip: true
    radius: 15

    PlasmaCore.IconItem {
        id: iconLocalRep
        anchors.fill: parent
        anchors.margins: 12

        visible: !iconRemote.visible
        source: "package-x-generic"
    }

    Image {
        id: iconRemote
        anchors.fill: parent
        anchors.margins: 12
        visible: iconRemote.status == Image.Ready
    }
}
