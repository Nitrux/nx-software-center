import QtQuick 2.0
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.nx.softwarecenter 1.0

PlasmaComponents.ListItem {

    height: Math.max(38, innerLayout.height)

    property string name
    property string description

    property bool isDownloadable
    property bool isDownloaded

    property string taskId
    property int taskProgress: -1

    signal requestRun
    signal requestRemove
    signal requestDownload

    property var task

    onTaskChanged: {
        if (task !== undefined) {
            progressBar.visible = true
            task.progressChanged.connect(hangleTaskProgressChanged)
            task.totalChanged.connect(hangleTaskTotalProgressChanged)
            task.stateChanged.connect(handleTaskStateChanged)
        } else
            progressBar.visible = false
    }


    function hangleTaskProgressChanged() {
        progressBar.value = task.progress
    }

    function hangleTaskTotalProgressChanged() {
        progressBar.maximumValue = task.total
    }

    function handleTaskStateChanged() {
        if (task.state !== Task.TASK_RUNNING && task.state !== Task.TASK_CREATED) {
            taskId = undefined
            task = undefined
        }
    }

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
            Layout.maximumWidth: 200
            wrapMode: Text.WordWrap

            text: name
        }

        PlasmaComponents.Label {
            Layout.fillWidth: true
            text: description

            wrapMode: Text.WordWrap
        }

        PlasmaComponents.Button {

            iconName: "emblem-select-add"
            text: i18n("Download")

            visible: !isDownloaded && taskId == ""
            onClicked: requestDownload()
        }

        PlasmaComponents.Button {
            iconName: "emblem-select-remove"
            text: i18n("Remove")

            visible: isDownloaded && taskId == ""
            onClicked: requestRemove()
        }

        PlasmaComponents.Button {
            iconName: "application-x-executable"
            visible: isDownloaded && taskId == ""
            text: i18n("Run")

            onClicked: requestRun()
        }

        PlasmaComponents.ProgressBar {
            id: progressBar
            Layout.maximumWidth: 120

            visible: false
        }
    }
}
