import QtQuick 2.5

import org.nx.softwarecenter 1.0

QtObject {
    property var contentLoader

    property var listDepartamentsRequest: undefined
    property var departamentsListModel: ListModel {
    }

    //    property var snapdClient: SnapdClient {
    //        id: snapdClient

    //        Component.onCompleted: {
    //            var request = connect();
    //        }
    //    }
    property var departaments: []
    function displayDepartaments() {
        showLoadingMessage()

        if (departamentsListModel.count == 0) {
            listDepartamentsRequest = SnapStore.listDepartments()

            listDepartamentsRequest.complete.connect(
                        onListDepartamentsRequestComplete)
            listDepartamentsRequest.runAsync()
        } else
            showDepartamentsView()
    }

    function showLoadingMessage() {
        contentLoader.source = "qrc:/PlaceHolderView.qml"
        var placeHolder = contentLoader.item
        if (placeHolder !== undefined) {
            placeHolder.showBusyIndicator = true
            placeHolder.message = i18n("Listing departaments, please wait ...")
        }
    }

    function showErrorMessage() {
        contentLoader.source = "qrc:/PlaceHolderView.qml"
        var placeHolder = contentLoader.item
        if (placeHolder !== undefined) {
            var message = listDepartamentsRequest.errorString
            if (message == "")
                message = textConstants.unknownError

            placeHolder.message = message
            placeHolder.iconName = "face-sad"
            placeHolder.showBusyIndicator = false
        }
    }

    function showDepartamentsView() {
        contentLoader.source = "qrc:/DepartamentsView.qml"
        var departamentsView = contentLoader.item
        if (departamentsView !== undefined) {
            departamentsView.departamentsListModel = departamentsListModel
        }
    }

    function onListDepartamentsRequestComplete() {
        var departments = []
        if (listDepartamentsRequest.error == 0) {
            for (var i = 0; i < listDepartamentsRequest.departamentCount(
                     ); i++) {
                departments.push(listDepartamentsRequest.departament(i))
            }

            departments.sort(function (a, b) {
                if (a.name < b.name)
                    return -1
                if (a.name > b.name)
                    return 1
                return 0
            })

            departamentsListModel.clear()
            for (var i = 0; i < departments.length; i++) {
                departamentsListModel.append(departments[i])
            }

            showDepartamentsView()
        } else {
            showErrorMessage()
        }
    }
}
