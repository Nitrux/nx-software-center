function prepare(settings) {

    function execute(update, success, error) {
        update(i18n("Loading default settings "))

        settings.store = settings.Ubuntu
        settings.customStoreUrl = ""
        settings.useProxy = false
        settings.httpProxy = ""
        settings.httpProxyPort = 0
        settings.httpsProxy = ""
        settings.httpsProxyPort = 0
        settings.noProxy = ""

        //        var request = settings.apply()
        //        request.finished.connect(onFinished)
        //        request.start()
        success(i18n("Default settings loaded."))
    }

    return {
        icon: "edit-undo-symbolic",
        text: i18n("Restore Defaults"),
        action: execute
    }
}
