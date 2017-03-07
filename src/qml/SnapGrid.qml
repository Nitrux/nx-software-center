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
    property int keyFocusIndex: -1

    Flickable {
        contentWidth: snapsList.width
        contentHeight: snapsList.height
        anchors.margins: 20


//        Rectangle {
//            anchors.fill: parent
//            color: snapsList.activeFocus ? "green" : "red"
//        }

        Flow {
            id: snapsList
            width: root.width - 20
            spacing: 12

            Repeater {
                id: repeater
            }

            focus: true
            Keys.onPressed: {
                if (event.key == Qt.Key_Right)
                    moveNext()
                else if (event.key == Qt.Key_Left)
                    moveRight()
                else if (event.key == Qt.Key_Down)
                    moveDown()
                else if (event.key == Qt.Key_Up)
                    moveUp()
                else if (event.key == Qt.Key_Space)
                    toggleSelection()
            }
        }
    }

    function moveNext() {

    }
}
