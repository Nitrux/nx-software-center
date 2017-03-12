import QtQuick 2.0

import Snapd 1.0
import org.nx.softwarecenter 1.0

QtObject {
    signal loadingLocalPackageInfo
    signal loadingStorePackageInfo

    signal localPackageInfoAvailable
    signal storePackageInfoAvailable

    property var localPackageInfo: undefined
    property var storePackageInfo: undefined

    property var snapdClient: SnapdClient {
    }

    property var p: QtObject {

        property string snap_name
        property int callsCount: 0

        function getStoreInfo() {
            var storeRequest = SnapStore.getSnapDetails(snap_name)
            storeRequest.complete.connect(function () {
                if (storeRequest.error == 0) {
                    storePackageInfo = storeRequest.snapDetails()
                    storePackageInfoAvailable()
                } else {
                    error()
                }
            })

            storeRequest.runAsync()
        }

        function getLocalInfo() {
            // Ensure we are connected
            var connectRequest = snapdClient.connect()
            connectRequest.runSync()

            var localRequest = snapdClient.listOne(snap_name)

            localRequest.complete.connect(function () {
                localPackageInfo = localRequest.snap()
                localPackageInfoAvailable()
            })

            localRequest.runAsync()
        }
    }

    function getInfo(snap_name) {
        p.snap_name = snap_name
        p.getLocalInfo()
        p.getStoreInfo()
    }

    function refreshInfo() {
        p.getLocalInfo()
        p.getStoreInfo()
    }
}
