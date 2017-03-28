import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import "qrc:/scripts/Utils.js" as UtilsJs

import org.nx.softwarecenter 1.0

ListModel {
    id: snapsModel
    property var selectedItems
    property var fetchSnapsFunc

    Component.onCompleted: {
        selectedItems = {}
    }
    function refresh(snaps) {
        if (snaps === undefined)
            return;

        var list = snaps

        function compareSnaps(a, b) {
            return a.name.localeCompare(b.name)
        }

        list.sort(compareSnaps)

        for (var i = count - 1; i >= 0; i--) {
            var snap = get(i)
            var idx = UtilsJs.binaryIndexOf(snap, compareSnaps, function () {
                return list.length
            }, function (i) {
                return list[i]
            })

            if (idx == -1) {
                delete selectedItems[snap.name]
                remove(i)
            }
        }

        for (var i = 0; i < list.length; i++) {
            var snap = list[i]

            var oldIdx = _indexOf(snap.name)
            if (oldIdx == -1)
                insert(i, snap)
            else {
                if (oldIdx != i)
                    move(oldIdx, i, 1)
                set(i, snap)
            }
        }
    }

    function _indexOf(name) {
        return UtilsJs.binaryIndexOf(name, function (a, b) {
            return a.name.localeCompare(b)
        }, function () {
            return count
        }, function (i) {
            return get(i)
        })
    }

    function getByName(name) {
        var idx = _indexOf(name)
        return snapsModel.get(idx)
    }

    function getSelectedItems() {
        return Object.keys(selectedItems)
    }
}
