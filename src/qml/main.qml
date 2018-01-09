import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.nxos.softwarecenter 1.0

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

        onGoStore: main.showSearchView()
        onGoTasks: main.showTasksView()
        onStoreQueryTyped: main.search(query)
    }

    footer: StatusArea {
        id: statusArea
        height: visible ? 42 : 0
        visible: false
    }

    StackView {
        id: stackView
        anchors.fill: parent
    }

    TextConstants {
        id: textConstants
    }

    Component.onCompleted: loadStoreContents()

    function search(query) {
        SearchController.search(query);
    }

    function loadStoreContents() {
        showBusyMessage("Loading store contents...");
        SearchController.fetchCompleted.connect(showSearchView);
        SearchController.fetchError.connect(showFetchErrorMessage);
        SearchController.fetch()
    }

    function showTasksView() {
        print("showTasksView")
        stackView.replace("qrc:/TasksView.qml")
    }

    function showSearchView() {
        print("showSearchView")
        stackView.replace("qrc:/SearchView.qml")
    }

    function showBusyMessage(message) {
        stackView.push("qrc:/PlaceHolderView.qml", {
                           message: message
                       })
    }

    function showFetchErrorMessage() {
        print("showFetchErrorMessage")
        stackView.replace("qrc:/PlaceHolderView.qml", {
                           message: textConstants.unknownError,
                           iconName: "face-sad",
                           showBusyIndicator: false
                       })
    }

    //    function goStore() {
    //        navigationPanel.currentView = "store"
    //        content.source = "qrc:/StoreView.qml"
    //    }

    //    function search(query) {
    //        goStore()
    //        SearchViewController.search(query)
    //    }

    //    Connections {
    //        target: SearchViewController
    //        onApplications: appsCache = apps
    //        onNoMatchFound: {
    //            appsCache = undefined
    //            main.showNothingFoundSreen()
    //        }
    //    }
    //    function showLoadingAppsScreen() {
    //        content.source = "qrc:/PlaceHolderView.qml"
    //        var placeHolder = content.item
    //        if (placeHolder !== undefined) {
    //            placeHolder.showBusyIndicator = true
    //            placeHolder.message = i18n("Fetching applications lists ...")
    //        }
    //    }

    //    function showLoadingScreen(message) {
    //        content.source = "qrc:/PlaceHolderView.qml"
    //        var placeHolder = content.item
    //        if (placeHolder !== undefined) {
    //            placeHolder.showBusyIndicator = true
    //            placeHolder.message = message
    //        }
    //    }

    //    function showError(message) {
    //        content.source = "qrc:/PlaceHolderView.qml"
    //        var placeHolder = content.item
    //        if (placeHolder !== undefined) {
    //            if (message == "")
    //                message = textConstants.unknownError

    //            placeHolder.message = message
    //            placeHolder.iconName = "face-sad"
    //            placeHolder.showBusyIndicator = false
    //        }
    //    }

    //    function showNothingFoundSreen() {
    //        if (main.refreshCacheTask != undefined)
    //            return

    //        content.source = "qrc:/PlaceHolderView.qml"
    //        var placeHolder = content.item
    //        if (placeHolder !== undefined) {
    //            placeHolder.message = textConstants.noApplicationsFound
    //            placeHolder.iconName = "dialog-information"
    //            placeHolder.showBusyIndicator = false
    //        }
    //    }

    //    Component.onCompleted: {
    //        refreshCache()
    //    }

    //    function refreshCache() {
    //        navigationPanel.disable()

    //        SearchController.fetchingApplications.connect(showLoadingScreen);
    //        SearchController.fetchCompleted.connect(goStore);
    //        SearchController.fetchError.connect(goStore);

    //        SearchController.searching.connect(showLoadingAppsScreen);
    //        SearchController.resultsReady.connect(goStore);

    //        SearchController.fetch()
    //        SearchController.search("")
    //    }

    //    function handleRefreshCacheFinished() {
    //        if (main.refreshCacheTask.state !== Task.TASK_RUNNING
    //                && main.refreshCacheTask.state !== Task.TASK_CREATED) {
    //            goStore()
    //            main.refreshCacheTask = undefined

    //            navigationPanel.enable()

    //            SearchViewController.search()
    //            goStore()
    //        }
    //    }
}
