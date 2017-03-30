import QtQuick 2.5

import org.nx.softwarecenter 1.0

QtObject {
    signal loading
    signal complete
    signal error(string message)

    property var departaments: []
    property var listDepartamentsRequest: undefined
    property var departamentsListModel: ListModel {
    }

    function run() {
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
        departaments = []
        if (listDepartamentsRequest.error == 0) {
            for (var i = 0; i < listDepartamentsRequest.departamentCount(
                     ); i++) {
                departaments.push(listDepartamentsRequest.departament(i))
            }

            departaments.sort(function (a, b) {
                if (a.name < b.name)
                    return -1
                if (a.name > b.name)
                    return 1
                return 0
            })

            departamentsListModel.clear()
            for (var i = 0; i < departaments.length; i++) {
                departamentsListModel.append(departaments[i])
            }

            complete()
        } else {
            var message = listDepartamentsRequest.errorString
            error(message)
        }
    }
}
