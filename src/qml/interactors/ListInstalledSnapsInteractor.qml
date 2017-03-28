import QtQml 2.2

import Snapd 1.0

QtObject {
    property var snaps: []

    signal loading;
    signal complete(var snaps);
    signal error(var message);

    property var snapdClient;

    property QtObject p: QtObject {
        property var snapdClient: SnapdClient {
        }

        function listInstalledSnaps() {
            var connectRequest = snapdClient.connect()
            connectRequest.runSync()

            var request = snapdClient.list()
            request.complete.connect( function () {
                snaps = []

                if (request.error) {
                    error(request.errorMessage);
                    return;
                }

                for (var i = 0; i < request.snapCount; i++) {
                    var snap = request.snap(i)
                    snaps.push(snap)
                }

                snaps.sort(function (a, b) { return a < b})
                complete(snaps)
            });

            request.runAsync()
        }
    }

    function listInstalledSnaps() {
        loading()
        p.listInstalledSnaps();
    }
}
