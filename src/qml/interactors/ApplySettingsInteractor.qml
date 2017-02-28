import QtQuick 2.5

import org.nx.softwarecenter 1.0

Interactor {
    property var settings
    property var job

    function start() {
        started()

        job = settings.apply()
        job.finished.connect(onFinished)
        job.start()
    }

    function onFinished() {
        // Handle error
        if (job && job.error !== 0) {

            // The user didn't input any valid password ?
            if (job.error === 4) {
                statusArea.dissmiss()
                finished()
            } else {
                var message = job.errorString
                if (message == "") {
                    console.log("Unknown error code: ", job.error)
                    message = textConstants.unknownError
                }

                statusArea.noticeError(message)
                failed()
            }
        } else {
            statusArea.noticeSuccess(i18n("Settings applied successfully"))
            finished()
        }
    }

    onStarted: statusArea.locked = true // lock status area for this job
}
