import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import Snapd 1.0

import org.nx.softwarecenter 1.0

import "qrc:/scripts/Utils.js" as Utils

Item {
    id: storeViewRoot

    property bool busy: false

    SnapdClient {
        id: snapdClient
    }

    StackView {
        id: contentLoader
        anchors.fill: parent

        initialItem: "qrc:/DepartamentsView.qml"


    }

    Component {
        id: statusView
        PlaceHolderView {
        }
    }

    Component.onCompleted: statusArea.clearContext()
}
