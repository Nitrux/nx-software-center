import QtQuick 2.5

import org.nx.softwarecenter 1.0

QtObject {
    property var contentLoader

    signal loading()
    signal complete()
    signal error(string message)

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
        loading()

        if (departamentsListModel.count == 0) {
            listDepartamentsRequest = SnapStore.listDepartments()

            listDepartamentsRequest.complete.connect(
                        onListDepartamentsRequestComplete)
            listDepartamentsRequest.runAsync()
        } else
            complete()
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

            complete()
        } else {
            var message = listDepartamentsRequest.errorString
            error(message)
        }
    }
}
