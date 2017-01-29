function callActionSeccuentially(action, targets, onUpdate, onFinished, onError) {
    if (targets.length === 0) {
        onFinished()
        return
    }

    var target = targets.pop()
    var request = action(target)

    onUpdate(target)

    function singleCallCompleted() {
        if (request.error !== 0) {
            onError(request.errorString)
            return
        }

        callActionSeccuentially(action, targets, onUpdate, onFinished, onError)
    }

    request.finished.connect(singleCallCompleted)
    request.start()
}

var MESSAGE_NO_SNAPS_SELECTED = i18n("No snaps selected.")
