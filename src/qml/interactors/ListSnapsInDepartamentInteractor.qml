import QtQuick 2.0

import org.nx.softwarecenter 1.0

QtObject {
    property string departament: undefined

    property var listDepartamentSnapsRequest: undefined
    property var snaps: []

    signal loading
    signal finished
    signal error(var message)

    function fetchSnaps() {
        if (departament === undefined) {
            console.error(
                        "Undefined snaps departament for ListSnapsInDepartamentInteractor")
            return
        }

        listDepartamentSnapsRequest = SnapStore.getDepartment(departament)

        listDepartamentSnapsRequest.runAsync()

        listDepartamentSnapsRequest.complete.connect(
                    onListDepartamentSnapsRequestComplete)

        loading()
    }

    function onListDepartamentSnapsRequestComplete() {
        if (listDepartamentSnapsRequest.error != 0)
            error(listDepartamentSnapsRequest.errorString)
        else {
            var pkgs = []
            for (var i = 0; i < listDepartamentSnapsRequest.packagesCount(
                     ); i++) {
                var pkg = listDepartamentSnapsRequest.package(i)
                pkgs.push(pkg)
            }

            snaps = pkgs
            finished()
        }
    }
}
