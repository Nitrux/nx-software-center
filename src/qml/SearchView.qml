    import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import Snapd 1.0

import org.nx.softwarecenter 1.0

import "qrc:/scripts/Utils.js" as Utils

import "parts" as Parts
import "interactors" as Interactors

Parts.View {
    id: viewRoot

    SnapdClient {
        id: snapdClient
    }

    function refreshContent() {
        // No need to refresh departaments, this rarely changes
        // listDepartamentsInteractor.run()

        storeSnapsModel.refreshActions()
    }

    Interactors.StoreSearchInteractor {
        id: storeSearchInteractor
        snapdClient: snapdClient

        onLoading: viewRoot.showLoadingScreen(i18n("Lonking for snaps like: \"") + storeSearchInteractor.query + "\"")
        onComplete: {
            storeSnapsModel.refresh(storeSearchInteractor.snaps)
            viewRoot.sourceComponent = component_SnapGrid
        }
        onError: showError(message)
    }

    function query( text ) {
        storeSearchInteractor.search(text);
    }

    Parts.SnapsModel {
        id: storeSnapsModel

        property bool busy: false
        property string query: ""
        property string statusMessage: i18n("Type what are you looking for ...")
        property string errorMessage: ""
        property string statusMessageIcon: "face-laughing"

        Component.onCompleted: refresh()

        function refreshActions() {
            var keys = Object.keys(selectedItems)
            if (keys.length > 0) {
                var installAction = {
                    icon: "package-install",
                    text: textConstants.actionInstallTitle,
                    action: function () {
                        var targets_names = Object.keys(
                                    storeSnapsModel.selectedItems)
                        var targets = []
                        for (var i in targets_names) {
                            var model = storeSnapsModel.getByName(
                                        targets_names[i])
                            targets.push({
                                             name: model.name,
                                             channel: model.channel
                                         })
                        }

                        installSnapInteractor.targets = targets
                        installSnapInteractor.finished.connect(
                                    refreshActions)

                        installSnapInteractor.start()
                    }
                }

                statusArea.updateContext("emblem-info",
                                         textConstants.availableActionsNotice,
                                         [installAction])
            } else
                statusArea.clearContext()
        }

        fetchSnapsFunc: function () {
            query = searchField.text

            // Ensure we are connected
            var connectRequest = snapdClient.connect()
            connectRequest.runSync()

            busy = true
            statusMessage = i18n("Lonking for snaps like: \"") + query + "\""
            var request = snapdClient.find(0, query)
            request.runSync()

            busy = false
            if (request.snapCount == 0) {
                statusMessage = i18n(
                            "No snaps where found with text \"") + query + "\" try something else."
                statusMessageIcon = "face-sad"
            }
            if (request.error) {
                errorMessage = i18n(
                            "There was an error while procesing your request. Please check your internet connection and try again.")
                statusMessageIcon = "face-uncertain"
            } else {
                errorMessage = ""
            }
            var list = []
            //            snapList.sort(function (a, b) { return a < b})
            for (var i = 0; i < request.snapCount; i++) {
                var snap = request.snap(i)
                list.push(snap)
            }

            return list
        }
    }

    Connections {
        target: searchField
        onEditingFinished: storeSnapsModel.refresh()
    }

    Component {
        id: component_SnapGrid
        SnapGrid {
            model: storeSnapsModel
            delegate: SnapElementDelegate {
                snap_name: name
                snap_version: version
                snap_size: model.downloadSize !== undefined ? downloadSize : -1
                snap_icon: icon
                onSelectedChanged: {
                    if (selected)
                        storeSnapsModel.selectedItems[name] = "true"
                    else
                        delete storeSnapsModel.selectedItems[name]

                    storeSnapsModel.refreshActions()
                }

                onClicked: main.showSnapDetails(name)
            }
        }
    }
}
