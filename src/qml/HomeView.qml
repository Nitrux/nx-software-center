import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.nx.softwarecenter 1.0

import "qrc:/scripts/Utils.js" as Utils

import "parts" as Parts
import Snapd 1.0

Item {
    id: homeViewRoot

    SnapdClient {
        id: snapdClient
    }

    objectName: "homeView"

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

                    installedSnapsModel.refreshActions()
                }

                onClicked: main.showSnapDetails(name)
//                onClicked: {
//                    contentLoader.push({
//                                           item: Qt.resolvedUrl(
//                                                     "SnapDetailsView.qml"),
//                                           properties: {
//                                               package_name: name,
//                                               dismissCallback: function () {
//                                                   contentLoader.pop()
//                                                   installedSnapsModel.refresh()
//                                                   installedSnapsModel.refreshActions()
//                                               }
//                                           }
//                                       })
//                }
            }
        }
    }
    StackView {
        id: contentLoader
        anchors.fill: parent
        initialItem: component_SnapGrid
    }

    Parts.SnapsModel {
        id: installedSnapsModel
        fetchSnapsFunc: function () {
            // Ensure we are connected
            var connectRequest = snapdClient.connect()
            connectRequest.runSync()

            var request = snapdClient.list()
            request.runSync()

            var list = []
            //            snapList.sort(function (a, b) { return a < b})
            for (var i = 0; i < request.snapCount; i++) {
                var snap = request.snap(i)
                list.push(snap)
            }

            return list
        }

        function refreshActions() {
            var keys = Object.keys(selectedItems)
            var disableAction = {
                icon: "package-broken",
                text: textConstants.actionDisableTitle,
                action: function () {
                    var targets = Object.keys(selectedItems)

                    disableSnapInteractor.targets = targets
                    disableSnapInteractor.finished.connect(
                                installedSnapsModel.refresh)
                    disableSnapInteractor.targetProcessed.connect(
                                installedSnapsModel.refresh)

                    disableSnapInteractor.start()
                }
            }

            var enableAction = {
                icon: "package-installed-updated",
                text: textConstants.actionEnableTitle,
                action: function () {
                    var targets = Object.keys(selectedItems)

                    enableSnapInteractor.targets = targets
                    enableSnapInteractor.finished.connect(
                                installedSnapsModel.refresh)
                    enableSnapInteractor.targetProcessed.connect(
                                installedSnapsModel.refresh)

                    enableSnapInteractor.start()
                }
            }

            var refreshAction = {
                icon: "package-upgrade",
                text: textConstants.actionRefreshTitle,
                action: function () {
                    var targets_names = Object.keys(selectedItems)
                    var targets = []
                    for (var i in targets_names) {
                        var model = installedSnapsModel.getByName(
                                    targets_names[i])
                        targets.push({
                                         name: model.name,
                                         channel: model.channel
                                     })
                    }

                    refreshSnapInteractor.targets = targets
                    refreshSnapInteractor.finished.connect(
                                installedSnapsModel.refresh)
                    refreshSnapInteractor.targetProcessed.connect(
                                installedSnapsModel.refresh)

                    refreshSnapInteractor.start()
                }
            }

            var removeAction = {
                icon: "package-remove",
                text: textConstants.actionRemoveTitle,
                action: function () {
                    var targets = Object.keys(selectedItems)

                    removeSnapInteractor.targets = targets
                    removeSnapInteractor.finished.connect(
                                installedSnapsModel.refresh)
                    removeSnapInteractor.targetProcessed.connect(
                                installedSnapsModel.refresh)

                    removeSnapInteractor.start()
                }
            }

            if (keys.length > 0) {
                var actions = [disableAction, enableAction, refreshAction, removeAction]
                statusArea.updateContext("documentinfo",
                                         i18n("Available actions"), actions)
            } else
                statusArea.clearContext()
        }
    }

    Component.onCompleted: statusArea.clearContext()
}
