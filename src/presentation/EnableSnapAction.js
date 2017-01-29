var q = Qt.include("qrc:/actions/ActionUtils.js")

function prepare(snapd, snapsModel) {

    function execute(update, success, error) {
        var targets = snapsModel.getSelectedItems()

        if (targets.length == 0) {
            success(MESSAGE_NO_SNAPS_SELECTED)
            return
        }

        function onUpdate(snap, datails) {
            update(i18n("Enabling ") + snap)
            snapsModel.refresh()
        }

        function onFinished() {
            snapsModel.refresh()
            success(i18n("Enable snaps completed"))
        }

        callActionSeccuentially(snapd.enable, targets, onUpdate,
                                onFinished, error)
    }

    return {
        icon: "package-installed-updated",
        text: i18n("Enable"),
        action: execute
    }
}
