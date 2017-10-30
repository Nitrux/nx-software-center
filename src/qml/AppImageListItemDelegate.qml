import QtQuick 2.0
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.nx.softwarecenter 1.0

PlasmaComponents.ListItem {

    height: Math.max(38, innerLayout.height);

    property string name;
    property string description;

    property bool isDownloadable;
    property bool isDownloaded;

    signal requestRun();
    signal requestRemove();
    signal requestDownload();

    Connections {
        target: SearchViewController
        onApplications: appImageHubListView.model = apps
        onNoMatchFound: {
            appImageHubListView.model = undefined
        }
    }

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
            Layout.maximumWidth: 200;
            wrapMode: Text.WordWrap

            text: name
        }

        PlasmaComponents.Label {
            Layout.fillWidth: true
            text: description

            wrapMode: Text.WordWrap

        }

        PlasmaComponents.Button {
            visible: !isDownloaded
            iconName: "emblem-select-add"
            text: i18n("Download")

            onClicked: requestDownload()
        }

        PlasmaComponents.Button {
            iconName: "emblem-select-remove"
            visible: isDownloaded
            text: i18n("Remove")

            onClicked: requestRemove()
        }

        PlasmaComponents.Button {
            iconName: "application-x-executable"
            visible: isDownloaded
            text: i18n("Run")

            onClicked: requestRun()
        }
    }

}
