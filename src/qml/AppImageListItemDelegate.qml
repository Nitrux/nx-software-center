import QtQuick 2.0
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

//import org.nx.softwarecenter 1.0

PlasmaComponents.ListItem {

    height: Math.max(38, innerLayout.height)

    property string codeName : app_name
    property string description: app_description

    property bool isDownloadable: true
    property bool isDownloaded

    property string taskId
    property int taskProgress: -1

    signal requestExecute
    signal requestRemove
    signal requestInstall
    signal requestRefresh

    RowLayout {
        id: innerLayout
        anchors.left: parent.left
        anchors.leftMargin: 18
        anchors.right: parent.right
        anchors.rightMargin: 12

        anchors.verticalCenter: parent.verticalCenter

        PlasmaComponents.Label {
            Layout.alignment: Qt.AlignTop
            Layout.preferredWidth: 200
            Layout.maximumWidth: 200
            wrapMode: Text.WordWrap

            text: codeName
        }

        PlasmaComponents.Label {
            Layout.fillWidth: true
            text: description

            wrapMode: Text.WordWrap
        }

        PlasmaComponents.Button {

            iconName: "emblem-select-add"
            text: i18n("Download")

            visible: !isDownloaded && taskId == "" && isDownloadable
            onClicked: requestInstall()
        }

        PlasmaComponents.Button {
            iconName: "emblem-select-remove"
            text: i18n("Remove")

            visible: isDownloaded && taskId == ""
            onClicked: requestRemove()
        }

        PlasmaComponents.Button {
            iconName: "Application-x-executable"
            visible: isDownloaded && taskId == ""
            text: i18n("Run")

            onClicked: requestExecute()
        }
    }
}
