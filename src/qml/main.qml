import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.nxos.softwarecenter 1.0

ApplicationWindow {
    id: main

    visible: true
    width: 900
    height: 700

    property var refreshCacheTask
    property var appsCache

    color: theme.backgroundColor

    header: NavigationPanel {
        id: navigationPanel

        onGoStore: main.handleGoStore()
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
        initialItem: PlaceHolderView

        function findItemByObjectName(name) {
            var item = stackView.find(function (item, index) {
                return item.objectName === name
            })
            return item
        }

        function goTo(name, component) {
            var itemInstance = findItemByObjectName(name);
            if (itemInstance) {
                print("pop " + name);
                stackView.pop(itemInstance)
            } else {
                print("push " + name);
                stackView.push(component, {objectName: name})
            }
        }
    }

    TextConstants {
        id: textConstants
    }

    function search(query) {
        SearchController.search(query)
    }

    function showTasksView() {
        main.title = "Tasks"
        stackView.goTo("tasksView", "qrc:/TasksView.qml");
    }

    Connections {
        target: UpdaterController
        onIsWorkingChanged: handleUpdaterIsWorkingChanged(isWorking)
    }

    function handleUpdaterIsWorkingChanged(isWorking) {
        print("isWorking: " + UpdaterController.isWorking)
        print("isReady: " + UpdaterController.isReady)
        if (navigationPanel.currentView == "store") {
            if (isWorking) {
                main.title = "Loading contents"
                showBusyMessage("Loading store contents...")
            } else {
                if (UpdaterController.isReady)
                    showSearchView();
                else
                    showUpdateErrorMessage();
            }
        }
    }

    function handleGoStore() {
        if (UpdaterController.isReady)
            showSearchView()
        else
            UpdaterController.update()
    }

    function showSearchView() {
        main.title = "Explore";
        stackView.goTo("searchView", "qrc:/SearchView.qml");
    }

    function showBusyMessage(message) {
        stackView.goTo("placeHolderView", "qrc:/PlaceHolderView.qml");
        var item = stackView.findItemByObjectName("placeHolderView");
        item.message = message
        item.iconName = "";
        item.showBusyIndicator = true;
    }

    function showUpdateErrorMessage() {
        stackView.goTo("placeHolderView", "qrc:/PlaceHolderView.qml");
        var item = stackView.findItemByObjectName("placeHolderView");

        item.message = textConstants.fetchError
        item.iconName = "network-wireless-disconnected";
        item.showBusyIndicator = false;
    }

    Component.onCompleted: handleGoStore()
}
