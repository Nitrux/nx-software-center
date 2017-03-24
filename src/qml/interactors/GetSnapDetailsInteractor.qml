import QtQuick 2.0

import Snapd 1.0
import org.nx.softwarecenter 1.0

QtObject {
    signal loadingLocalPackageInfo
    signal loadingStorePackageInfo

    signal localPackageInfoAvailable
    signal storePackageInfoAvailable

    signal error

    property QtObject details: QtObject {
        property string package_name: ""
        property string channel: ""
        property string local_revision: ""
        property string store_revision: ""
        property string local_version: ""
        property string store_version: ""
        property int status: 0

        property string name: ""
        property string description: ""
        property var keywords: undefined
        property string license: ""
        property string publisher: ""
        property int ratings_average: -1
        property var screenshot_urls: []
        property var installed_size: undefined
        property var download_size: undefined
        property string icon: ""
        property var iconByteArray: undefined
    }

    property var snapdClient: SnapdClient {
    }

    property var p: QtObject {

        property string snap_name
        property int callsCount: 0

        function getStoreInfo() {
            var storeRequest = SnapStore.getSnapDetails(snap_name)
            storeRequest.complete.connect(function () {
                if (storeRequest.error == 0) {
                    var data = storeRequest.snapDetails()

                    details.package_name = data.package_name
                    details.channel = data.channel
                    details.store_revision = data.revision

                    details.name = data.title
                    details.description = data.description
                    details.keywords = data.keywords
                    details.licence = data.license
                    details.publisher = data.publisher
                    details.ratings_average = data.ratings_average
                    details.screenshot_urls = data.screenshot_urls
                    details.icon = data.icon_url

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
                var data = localRequest.snap()

                details.package_name = data.name
                details.channel = data.channel
                details.local_revision = data.revision
                details.local_version = data.version
                details.status = data.status

                details.name = data.name
                details.description = data.description
                details.installed_size = data.installedSize
                details.publisher = data.developer

                localPackageInfoAvailable()
            })

            localRequest.runAsync()
        }

        function getIcon() {
            // Ensure we are connected
            var connectRequest = snapdClient.connect()
            connectRequest.runSync()

            var request = snapdClient.getIcon(snap_name)

            request.complete.connect(function () {
                console.warn("SnapClient.getIcon is buggy and will by disabled by the moment")
//                var icon = request.icon()

//                if (icon) {
//                    details.iconByteArray = icon.data()
//                    print(details.iconByteArray )
//                }
            })
            request.runAsync()
        }
    }

    function getInfo(snap_name) {
        p.snap_name = snap_name
        p.getLocalInfo()
        p.getStoreInfo()
        p.getIcon()
    }

    function refreshInfo() {
        p.getLocalInfo()
        p.getStoreInfo()
    }
}
