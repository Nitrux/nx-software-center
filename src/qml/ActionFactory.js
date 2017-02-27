//  Description:
//      Build a bulk action object to be shown in the status bar. The action is triggered by
//      calling the "execute" function, then it will call "processTargetFunc" secuentally
//      on every item returned by "fetchTargetsFunc".
//
// Params:
//      title: text to be shown in the action button
//      icon: icon to be shown in the action button
//      fetchTargetsFunc(): function that must return a list with the targets to be processed
//      processTargetFunc(target): function to be called upon each target, it sould return a request object with
//                          that must emit the signals: "update" and "finished"
//      onUpdateFunc(message, part, total): function to be called when an update ocours
//      onErrorFunc(message): function to be called when an error ocours, the bulk action is stoped
//      onFinishedFunc(message): function to be called when all targets were processed
//
// Returns:
//      An action object with the properties: icon, title and execute.
function prepareBulkRequestAction(title, icon, fetchTargetsFunc, processTargetFunc, onUpdateFunc, onErrorFunc, onFinishedFunc) {

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

            callActionSeccuentially(action, targets, onUpdate,
                                    onFinished, onError)
        }

        request.finished.connect(singleCallCompleted)
        request.start()
    }

    function execute(update, success, error) {
        var targets = fetchTargetsFunc()

        if (targets.length == 0) {
            onFinishedFunc(MESSAGE_NO_SNAPS_SELECTED)
            return
        }

        callActionSeccuentially(processTargetFunc, targets, onUpdateFunc,
                                onFinishedFunc, onErrorFunc)
    }

    return {
        icon: icon,
        text: title,
        action: execute
    }
}

//  Description:
//      Build a simple action object to be shown in the status bar. The action is triggered by
//      calling the "execute" function, then it will call "processTargetFunc" on the item
//      returned by "fetchTargetFunc".
//
// Params:
//      title: text to be shown in the action button
//      icon: icon to be shown in the action button
//      fetchTargetFunc(): function that must return the target to be processed
//      processTargetFunc(target): function to be called upon the target, it sould return a request object with
//                          that must emit the signals: "update" and "finished"
//      onUpdateFunc(message, part, total): function to be called when an update ocours
//      onErrorFunc(message): function to be called when an error ocours
//      onFinishedFunc(message): function to be called when all targets were processed
//
// Returns:
//      An action object with the properties: icon, title and execute.
function prepareSimpleRequestAction(title, icon, fetchTargetFunc, processTargetFunc, onUpdateFunc, onErrorFunc, onFinishedFunc) {

    function execute(update, success, error) {
        var target = fetchTargetFunc()
        var request = processTargetFunc(target)

        // Provisional update notification until progess reports be implemented in request
        update(title + " " + target.name)

        function onRequestFinished() {
            if (request.error !== 0) {
                error(request.errorString)
                onErrorFunc(request.errorString)
            } else {
                success(i18n("Done ") + title + " " + target.name)
                onFinishedFunc()
            }
        }

        function onJobNewData(newData) {
            if (newData.status)
                update(newData.status)
        }


        if (request.onNewData)
            request.onNewData.connect(onJobNewData)

        request.finished.connect(onRequestFinished)
        request.start()
    }

    return {
        icon: icon,
        text: title,
        action: execute
    }
}
