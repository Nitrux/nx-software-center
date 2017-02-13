import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import Snapd 1.0

import org.nx.softwarecenter 1.0

import "qrc:/scripts/Utils.js" as Utils
import "qrc:/actions/InstallSnapAction.js" as InstallSnapAction

Item {
    SnapsModel {
        id: storeSnapsModel

        property bool busy: false
        property string query: ""
        property string statusMessage: i18n("Type what are you looking for ...")
        property string errorMessage: ""
        property string statusMessageIcon: "face-laughing"

        Component.onCompleted: refresh()

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

    SnapGrid {
        model: storeSnapsModel
        delegate: SnapElementDelegate {
            snap_name: name
            snap_version: version
            snap_size: downaloadSize
            onSelectedChanged: {
                if (selected)
                    storeSnapsModel.selectedItems[name] = "true"
                else
                    delete storeSnapsModel.selectedItems[name]
            }
        }
    }
}
