import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.nx.softwarecenter 1.0

import "qrc:/scripts/Utils.js" as Utils

import "parts" as Parts
import "interactors" as Interactors
import Snapd 1.0

Parts.View {
    id: viewRoot
    objectName: "homeView"

    SnapdClient {
        id: snapdClient
    }

    Interactors.ListInstalledSnapsInteractor {
        id: listInstalledSnapsInteractor

        snapdClient: snapdClient
        onLoading: {
            if (snaps.length == 0) {
                viewRoot.showLoadingScreen(
                            i18n("Listing installed snaps, please wait"))
            }
        }
        onComplete: {
            installedSnapsModel.refresh(snaps)
            if (viewRoot.sourceCompnent !== component_SnapGrid)
                viewRoot.sourceComponent = component_SnapGrid

            viewRoot.refreshActions()
        }
        onError: showError(
                     i18n("Unable to fetch system snaps, plaease by sure "
                          + "that the snapd service is available and running"))
    }

    function refreshContent () {
        listInstalledSnapsInteractor.listInstalledSnaps()
    }

    Parts.SnapsModel {
        id: installedSnapsModel
    }

    Component {
        id: component_SnapGrid
        SnapGrid {
            model: installedSnapsModel
            delegate: SnapElementDelegate {
                snap_name: name
                snap_version: version
                snap_size: installedSize
                isDisabled: status != 4

                onSelectedChanged: {
                    if (selected)
                        installedSnapsModel.selectedItems[name] = "true"
                    else
                        delete installedSnapsModel.selectedItems[name]

                    viewRoot.refreshActions()
                }

                onClicked: main.showSnapDetails(name)
            }
        }
    }

    Component.onCompleted: {
        statusArea.clearContext()
        listInstalledSnapsInteractor.listInstalledSnaps()
    }

    function refreshActions() {

        var disableAction = {
            icon: "package-broken",
            text: textConstants.actionDisableTitle,
            action: function () {
                var targets = installedSnapsModel.getSelectedItems()

                disableSnapInteractor.targets = targets
                disableSnapInteractor.finished.connect(viewRoot.refreshContent)
                disableSnapInteractor.targetProcessed.connect(viewRoot.refreshContent)

                disableSnapInteractor.start()
            }
        }

        var enableAction = {
            icon: "package-installed-updated",
            text: textConstants.actionEnableTitle,
            action: function () {
                var targets = Object.keys(installedSnapsModel.selectedItems)

                enableSnapInteractor.targets = targets
                enableSnapInteractor.finished.connect(viewRoot.refreshContent)
                enableSnapInteractor.targetProcessed.connect(viewRoot.refreshContent)

                enableSnapInteractor.start()
            }
        }

        var refreshAction = {
            icon: "package-upgrade",
            text: textConstants.actionRefreshTitle,
            action: function () {
                var targets_names = Object.keys(
                            installedSnapsModel.selectedItems)
                var targets = []
                for (var i in targets_names) {
                    var model = installedSnapsModel.getByName(targets_names[i])
                    targets.push({
                                     name: model.name,
                                     channel: model.channel
                                 })
                }

                refreshSnapInteractor.targets = targets
                refreshSnapInteractor.finished.connect(viewRoot.refreshContent)
                refreshSnapInteractor.targetProcessed.connect(viewRoot.refreshContent)

                refreshSnapInteractor.start()
            }
        }

        var removeAction = {
            icon: "package-remove",
            text: textConstants.actionRemoveTitle,
            action: function () {
                var targets = Object.keys(selectedItems)

                removeSnapInteractor.targets = targets
                removeSnapInteractor.finished.connect(viewRoot.refreshContent)
                removeSnapInteractor.targetProcessed.connect(viewRoot.refreshContent)

                removeSnapInteractor.start()
            }
        }

        var keys = installedSnapsModel.getSelectedItems()
        if (keys.length > 0) {
            var actions = [disableAction, enableAction, refreshAction, removeAction]
            statusArea.updateContext("emblem-info",
                                     i18n("Available actions"), actions)
        } else
            statusArea.clearContext()
    }
}
