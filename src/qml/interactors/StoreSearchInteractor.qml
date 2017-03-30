import QtQuick 2.0

Item {
    property var snapdClient
    property var snaps: []
    property string query;

    signal loading
    signal complete(var snaps)
    signal error(var message)

    function search(text) {
        if (snapdClient === undefined) {
            console.log("snapdClient is undefined")
            return
        }
        query = text;
        loading()

        // Ensure we are connected
        var connectRequest = snapdClient.connect()
        connectRequest.runSync()

        var request = snapdClient.find(0, query)
        request.complete.connect(function () {
            if (request.error) {
                error(i18n("There was an error while procesing your request. "
                           + "Please check your internet connection and try again."))
                return;
            }

            snaps = []
            for (var i = 0; i < request.snapCount; i++) {
                var snap = request.snap(i)
                snaps.push(snap)
            }

            complete(snaps)
        })
        request.runAsync()
    }
}
