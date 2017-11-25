import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.nx.softwarecenter 1.0

ApplicationWindow {
    id: main
    title: qsTr("NX Software Center")

    visible: true
    width: 900
    height: 700

    property var refreshCacheTask
    property var appsCache

    color: theme.backgroundColor

    //    visibility: Qt.WindowFullScreen
    header: NavigationPanel {
        id: navigationPanel

        onGoHome: main.goHome()
        onStoreQueryTyped: main.search(query)
    }

    footer: StatusArea {
        id: statusArea
        height: visible ? 42 : 0
        visible: false
    }

    Loader {
        id: content
        anchors.fill: parent
    }

    TextConstants {
        id: textConstants
    }

    function goStore() {
        navigationPanel.currentView = "store"
        content.source = "qrc:/StoreView.qml"
    }

    function search(query) {
        goStore()
        SearchViewController.search(query)
    }

    Connections {
        target: SearchViewController
        onApplications: appsCache = apps
        onNoMatchFound: {
            appsCache = undefined
            main.showNothingFoundSreen()
        }
    }

    function showLoadingScreen(message) {
        content.source = "qrc:/PlaceHolderView.qml"
        var placeHolder = content.item
        if (placeHolder !== undefined) {
            placeHolder.showBusyIndicator = true
            placeHolder.message = message
        }
    }

    function showError(message) {
        content.source = "qrc:/PlaceHolderView.qml"
        var placeHolder = content.item
        if (placeHolder !== undefined) {
            if (message == "")
                message = textConstants.unknownError

            placeHolder.message = message
            placeHolder.iconName = "face-sad"
            placeHolder.showBusyIndicator = false
        }
    }

    function showNothingFoundSreen() {
        if (main.refreshCacheTask != undefined)
            return

        content.source = "qrc:/PlaceHolderView.qml"
        var placeHolder = content.item
        if (placeHolder !== undefined) {
            placeHolder.message = textConstants.noApplicationsFound
            placeHolder.iconName = "dialog-information"
            placeHolder.showBusyIndicator = false
        }
    }

    Component.onCompleted: {
        refreshCache()
    }

    function refreshCache() {
        navigationPanel.disable()

        var taskId = TasksController.fetchApps()
        main.refreshCacheTask = TasksController.getTask(taskId)

        showLoadingScreen("Loading aplications...")
        main.refreshCacheTask.stateChanged.connect(handleRefreshCacheFinished)
    }

    function handleRefreshCacheFinished() {
        if (main.refreshCacheTask.state !== Task.TASK_RUNNING
                && main.refreshCacheTask.state !== Task.TASK_CREATED) {
            goStore()
            main.refreshCacheTask = undefined

            navigationPanel.enable()

            SearchViewController.search()
            goStore()
        }
    }
}
