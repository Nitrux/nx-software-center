import QtQuick 2.0

Item {
    property var statusPanel
    property alias source: loader.source
    property alias sourceComponent: loader.sourceComponent

    Loader {
        id: loader
        anchors.fill: parent
    }

    // Must by reimplemented by views
    function refreshContent() {
        console.log("Refreshing view: ", objectName)
    }

    // Must by reimplemented by views
    function updateActions() {
        if (statusPanel === undefined) {
            console.error("Unable to access statusPanel at " + objectName
                          + ". Please ensure that it's initialized propoertly")
            return;
        }

        statusPanel.clear()
        statusPanel.notice("Displaying view " + objectName)
    }

    function showLoadingScreen(message) {
        loader.source = "qrc:/PlaceHolderView.qml"
        var placeHolder = loader.item
        if (placeHolder !== undefined) {
            placeHolder.showBusyIndicator = true
            placeHolder.message = message
        }
    }

    function showError(message) {
        loader.source = "qrc:/PlaceHolderView.qml"
        var placeHolder = loader.item
        if (placeHolder !== undefined) {
            if (message == "")
                message = textConstants.unknownError

            placeHolder.message = message
            placeHolder.iconName = "face-sad"
            placeHolder.showBusyIndicator = false
        }
    }
}
