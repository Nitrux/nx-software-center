import QtQuick 2.5

import org.nx.softwarecenter 1.0

Interactor {
    property var targets
    property bool multipleTargets: false
    property var currentJob

    signal targetProcessed(string target)

    function start() {
        started()

        if (targets === undefined) {
            finished()
            return
        }

        multipleTargets = targets.length > 1

        statusArea.notice(textConstants.requestCredentialNotice, [])
        processTargets()
    }

    function processTargets() {
        statusArea.jobRunning = false

        // Handle error
        if (currentJob && currentJob.error !== 0) {

            // The user didn't input any valid password ?
            if (currentJob.error === 4) {
                statusArea.dissmiss()
                finished()
            } else {
                var message = currentJob.errorString
                if (message == "") {
                    console.log("Unknown error code: ", currentJob.error)
                    message = textConstants.unknownError
                }

                statusArea.noticeError(message)
                failed()
            }

            return
        }

        // handle finished
        if (targets.length === 0) {
            var message = ""
            if (multipleTargets)
                message = i18n("All snaps where removed successfully!")
            else
                message = i18n("Snap removed successfully!")

            statusArea.noticeSuccess(message)
            finished()

            return
        }

        // process next target
        var target = targets.pop()
        statusArea.jobRunning = true
        currentJob = SnapdRootClient.remove (target)

        currentJob.newData.connect(function (newData) {
            statusArea.notice(newData.status, [])
        })
        currentJob.finished.connect(processTargets)

        currentJob.finished.connect(function () {
            targetProcessed(target)
        })

        currentJob.start()
    }

    onStarted: statusArea.locked = true // lock status area for this job
}
