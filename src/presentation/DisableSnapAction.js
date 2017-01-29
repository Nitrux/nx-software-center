var q = Qt.include("qrc:/actions/ActionUtils.js")

function prepare(snapd, snapsModel) {

    function execute(update, success, error) {
        var targets = snapsModel.getSelectedItems()

        if (targets.length == 0) {
            success(MESSAGE_NO_SNAPS_SELECTED)
            return
        }

        function onUpdate(snap, datails) {
            update(i18n("Disabling ") + snap)
            snapsModel.refresh()
        }

        function onFinished() {
            snapsModel.refresh()
            success(i18n("Disable snaps completed"))
        }

        callActionSeccuentially(snapd.disable, targets, onUpdate,
                                onFinished, error)
    }

    return {
        icon: "package-broken",
        text: i18n("Disable"),
        action: execute
    }
}
