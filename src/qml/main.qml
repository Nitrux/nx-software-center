import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import "interactors" as Interactors

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: qsTr("NX Software Center")
    id: main

    Rectangle {
        id: background
        anchors.fill: parent
        color: "#EEEEEE"
    }
    TextConstants {
        id: textConstants
    }

    ColumnLayout {
        anchors.fill: parent
        NavigationPanel {
            id: navigationPanel
            Layout.fillWidth: true

            onGoHome: main.goHome()
            onGoStore: showDepartamensView()
            onGoSettings: showSettings()
            onStoreQueryTyped: main.showSearchView(query)
        }

        StackView {
            id: content
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 400
        }

        StatusArea {
            id: statusArea
            Layout.maximumHeight: statusArea.visible ? 38 : 0
            Layout.preferredHeight: statusArea.visible ? 38 : 0
            Layout.fillWidth: true
            Layout.bottomMargin: 4
        }
    }


    // App Interactors
    Interactors.DisableSnapInteractor {
        id: disableSnapInteractor
    }

    Interactors.EnableSnapInteractor {
        id: enableSnapInteractor
    }

    Interactors.RefreshSnapInteractor {
        id: refreshSnapInteractor
    }

    Interactors.RemoveSnapInteractor {
        id: removeSnapInteractor
    }

    Interactors.InstallSnapInteractor {
        id: installSnapInteractor
    }

    Interactors.GetSnapDetailsInteractor {
        id: showSnapDetailsInteractor

        function goBack() {
            content.pop(StackView.Immediate)
            content.currentItem.refreshContent()
        }
        onLoadingLocalPackageInfo: showLoadingScreen( i18n("Fetching snap info, please wait ..."))
        onLoadingStorePackageInfo: showLoadingScreen( i18n("Fetching snap info, please wait ..."))

        onLocalPackageInfoAvailable: showDetailsView()
        onStorePackageInfoAvailable: showDetailsView()


        function showDetailsView() {
            if (content.currentItem.objectName != "snapDetailsView") {
                if (content.currentItem.objectName == "placeHolderView")
                    content.replace("qrc:/SnapDetailsView.qml", {"snap": showSnapDetailsInteractor.details}, StackView.Immediate)
                else
                    content.push("qrc:/SnapDetailsView.qml", {"snap": showSnapDetailsInteractor.details}, StackView.Immediate)
            }
            var detailsView = content.currentItem
            if (detailsView !== undefined) {
//                detailsView.snap = showSnapDetailsInteractor.details
                detailsView.updateContext()
                detailsView.refresh.connect(refreshInfo)
                detailsView.dismiss.connect(goBack)
            }
        }
    }

    function showSnapDetails(snap_name) {
        showSnapDetailsInteractor.getInfo(snap_name)
    }

    function goHome() {
        content.replace("qrc:/HomeView.qml", StackView.Immediate)
    }

    function showDepartamensView() {
        content.replace("qrc:/DepartamentsView.qml", StackView.Immediate)
    }

    function showSearchView(query) {
        content.replace("qrc:/SearchView.qml", StackView.Immediate)

        content.currentItem.query(query)
    }

    function showSettings() {
        content.replace("qrc:/SettingsView.qml", StackView.Immediate)
    }

    function showLoadingScreen(message) {
        content.replace("qrc:/PlaceHolderView.qml", StackView.Immediate)
        var placeHolder = content.currentItem
        if (placeHolder !== undefined) {
            placeHolder.showBusyIndicator = true
            placeHolder.message = message
        }
    }

    function showError(message) {
        content.replace("qrc:/PlaceHolderView.qml", StackView.Immediate)
        var placeHolder = content.currentItem
        if (placeHolder !== undefined) {
            if (message == "")
                message = textConstants.unknownError

            placeHolder.message = message
            placeHolder.iconName = "face-sad"
            placeHolder.showBusyIndicator = false
        }
    }

    Component.onCompleted: main.goHome()
}
