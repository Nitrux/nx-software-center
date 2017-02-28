import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import Snapd 1.0

import org.nx.softwarecenter 1.0

import "qrc:/scripts/Utils.js" as Utils

ScrollView {
    id: root
    property alias model: repeater.model
    property alias delegate: repeater.delegate
    Flickable {
        contentWidth: snapsList.width
        contentHeight: snapsList.height
        anchors.margins: 20

        Flow {
            id: snapsList
            width: root.width - 20

            Repeater {
                id: repeater
            }
        }
    }
}
