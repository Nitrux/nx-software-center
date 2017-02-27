function prepare(settings) {

    function execute(update, success, error) {

        update(i18n("Applying settings "))

        function onFinished() {
            if (request.error !== 0)
                error(request.errorString())
            else
                success(i18n("Settings applied successfully"))
        }

        var request = settings.apply()
        request.finished.connect(onFinished)
        request.start()
    }

    return {
        icon: "emblem-ok-symbolic",
        text: i18n("Apply"),
        action: execute
    }
}
