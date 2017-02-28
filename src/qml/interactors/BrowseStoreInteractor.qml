import QtQuick 2.5

import org.nx.softwarecenter 1.0

Interactor {
    property var settings
    property var job

    function start() {
        started()

        settings.store = settings.Ubuntu
        settings.customStoreUrl = ""
        settings.useProxy = false
        settings.httpProxy = ""
        settings.httpProxyPort = 0
        settings.httpsProxy = ""
        settings.httpsProxyPort = 0
        settings.noProxy = ""

        statusArea.dissmiss()
        finished()
    }

    onStarted: statusArea.locked = true // lock status area for this job
}
