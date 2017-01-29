var q = Qt.include("qrc:/actions/ActionUtils.js")

function prepare(snapd, snapsModel) {

    function execute(update, success, error) {
        var targets = snapsModel.getSelectedItems()

        if (targets.length == 0) {
            success(MESSAGE_NO_SNAPS_SELECTED)
            return
        }

        function onUpdate(snap, datails) {
            update(i18n("Refreshing ") + snap)
            snapsModel.refresh()
        }

        function onFinished() {
            snapsModel.refresh()
            success(i18n("Refresh snaps completed"))
        }

        function actionCaller(snap) {
            var model = snapsModel.getByName(snap)
            return snapd.refresh(snap, model.channel)
        }

        callActionSeccuentially(actionCaller, targets, onUpdate,
                                onFinished, error)
    }

    return {
        icon: "package-upgrade",
        text: i18n("Refresh"),
        action: execute
    }
}
