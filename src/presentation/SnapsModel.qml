import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.nx.softwarecenter 1.0

ListModel {
    id: snapsModel
    property var selectedItems
    property var fetchSnapsFunc

    Component.onCompleted: {
        selectedItems = {

        }
        refresh()
    }
    function refresh() {
        selectedItems = {

        }

        snapsModel.clear()
        var list = fetchSnapsFunc()

        list.sort(function (a, b) {
            return a.name.localeCompare(b.name)
        })
        for (var i = 0; i < list.length; i++) {
            var snap = list[i]
            snapsModel.append(snap)
        }
    }

    function getByName(name) {
        // TODO: make a binary search here
        for (var i = 0; i < count; i++) {
            var snap = get(i)
            if (snap.name == name)
                return snap
        }
    }

    function getSelectedItems() {
        return Object.keys(selectedItems)
    }
}
