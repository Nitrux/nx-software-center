import QtQuick 2.0

import org.nx.softwarecenter 1.0

QtObject {
    property string departament: undefined
    property var snaps: []

    signal loading
    signal finished
    signal error(var message)

    property var p: QtObject {
        property var request: undefined

        function fetchSnaps() {
            if (departament === undefined) {
                console.error("Undefined departament at ListSnapsInDepartamentInteractor")
                return
            }

            loading()
            if (request) {
                request.cancel()
                request.complete.disconnect(onRequestComplete)
            }

            request = SnapStore.getDepartment(departament)
            request.complete.connect(onRequestComplete)

            request.runAsync()
        }

        function onRequestComplete() {
            if (request.error != 0)
                error(request.errorString)
            else {
                var pkgs = []
                for (var i = 0; i < request.packagesCount(); i++) {
                    var pkg = request.package(i)
                    pkgs.push(pkg)
                }

                snaps = pkgs
                finished()
            }

            request = undefined
        }
    }

    function run() {
        p.fetchSnaps()
    }
}
