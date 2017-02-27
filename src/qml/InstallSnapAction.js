Qt.include("qrc:/actions/ActionUtils.js")

function prepare(snapd, snapsModel) {

    function execute(update, success, error) {
        var targets = snapsModel.getSelectedItems()

        if (targets.length == 0) {
            success(MESSAGE_NO_SNAPS_SELECTED)
            return
        }

        function onUpdate(snap, datails) {
            update(i18n("Installing ") + snap)
            snapsModel.refresh()
        }

        function onFinished() {
            snapsModel.refresh()
            success(i18n("Install snaps completed"))
        }

        function actionCaller(snap) {
            console.log("installing ", snap)
            var model = snapsModel.getByName(snap)
            return snapd.install(snap, "stable")
        }

        callActionSeccuentially(actionCaller, targets, onUpdate,
                                onFinished, error)
    }

    return {
        icon: "package-install",
        text: i18n("&Install"),
        action: execute
    }
}
