import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.nx.softwarecenter 1.0

import "qrc:/actions/DisableSnapAction.js" as DisableSnapAction
import "qrc:/actions/EnableSnapAction.js" as EnableSnapAction
import "qrc:/actions/RefreshSnapAction.js" as RefreshSnapAction
import "qrc:/actions/RemoveSnapAction.js" as RemoveSnapAction

import "qrc:/scripts/Utils.js" as Utils

import Snapd 1.0

Item {
    id: homeViewRoot

    SnapdClient {
        id: snapdClient
    }

    objectName: "homeView"

    SnapGrid {
        anchors.fill: parent
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
            }
        }
    }

    SnapsModel {
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
    }

    Component.onCompleted: {
        var actions = [DisableSnapAction.prepare(
                           SnapdRootClient,
                           installedSnapsModel), EnableSnapAction.prepare(
                           SnapdRootClient,
                           installedSnapsModel), RefreshSnapAction.prepare(
                           SnapdRootClient,
                           installedSnapsModel), RemoveSnapAction.prepare(
                           SnapdRootClient, installedSnapsModel)]
        statusArea.updateContext("documentinfo",
                                 i18n("Available actions"), actions)
    }
}
