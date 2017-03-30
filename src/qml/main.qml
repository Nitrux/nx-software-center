import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.2 as QtControls
import QtQuick.Controls.Styles.Plasma 2.0 as Styles

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import QtQuick.Controls.Styles.Plasma 2.0 as Styles
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.nx.softwarecenter 1.0

import Snapd 1.0

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
            onGoStore: browseStoreInteractor.displayDepartaments()
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

            onVisibleChanged: print(visible)
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



    Interactors.BrowseStoreInteractor {
        id: browseStoreInteractor
        contentLoader: content

        onLoading: showLoadingScreen(i18n("Listing departaments, please wait ..."))
        onError: showError(message)

        onComplete: {
            content.replace("qrc:/DepartamentsView.qml", StackView.Immediate)
            var departamentsView = content.currentItem
            if (departamentsView !== undefined) {
                departamentsView.departamentsListModel = departamentsListModel
            }
        }
    }

    Interactors.GetSnapDetailsInteractor {
        id: showSnapDetailsInteractor

        function goBack() {
            content.pop(StackView.Immediate)
            content.currentItem.refresh()
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
