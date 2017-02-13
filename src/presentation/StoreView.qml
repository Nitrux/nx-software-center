import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import Snapd 1.0

import org.nx.softwarecenter 1.0

import "qrc:/scripts/Utils.js" as Utils
import "qrc:/actions/InstallSnapAction.js" as InstallSnapAction

Item {
    id: storeViewRoot

    property bool busy: false

    SnapdClient {
        id: snapdClient
    }

    objectName: "storeView"

    //    Rectangle {
    //        color: "lightblue"
    //        anchors.fill: parent
    //        opacity: 0.1
    //    }
    Loader {
        id: contentLoader
        anchors.fill: parent

        //        sourceComponent:
        //        sourceComponent: storeSnapsModel.busy
        //                         || storeSnapsModel.count == 0 ? searchView : snapsView
    }

    Component {
        id: statusView
        Item {
            PlasmaComponents.Label {
                id: messageText
                anchors.top: busyModelIndicator.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                width: 400
                height: 100

                text: storeSnapsModel.errorMessage
                      == "" ? storeSnapsModel.statusMessage : storeSnapsModel.errorMessage

                fontSizeMode: Text.Fit
                horizontalAlignment: Text.AlignHCenter
                wrapMode: Text.WordWrap
                minimumPixelSize: 10
                font.pixelSize: 20
            }

            PlasmaComponents.BusyIndicator {
                id: busyModelIndicator
                visible: storeSnapsModel.busy
                anchors.centerIn: parent
            }

            PlasmaCore.IconItem {
                anchors.centerIn: parent
                anchors.verticalCenterOffset: -20
                width: 64
                height: 64

                source: storeSnapsModel.statusMessageIcon
                visible: !busyModelIndicator.visible
            }
        }
    }

    DepartamentsView {
        id: departamentsView
        anchors.fill: parent

        departamentsModel:departamentsModel
    }

    ListModel {
                id: departamentsModel

                Component.onCompleted: {
                    var request = SnapStore.listDepartments()

                    var onGetDepartamentFinished = function () {
                        print("finished")
                    }

                    var onListDepartamentsFinished = function () {
                        departamentsModel.clear()
                        print("departamentCount", request.departamentCount())

                        var departments = []
                        for (var i = 0; i < request.departamentCount(); i++) {
                            departments.push(request.departament(i))
                        }

                        departments.sort(function (a, b) {
                            if (a.name < b.name)
                                return -1
                            if (a.name > b.name)
                                return 1
                            return 0
                        })

                        for (var i = 0; i < departments.length; i++) {
                            departamentsModel.append(departments[i])
                        }
                    }



                    request.finished.connect(onListDepartamentsFinished)

                    request.start()
                    print(request)
                }
            }

    Component.onCompleted: {

        var actions = [InstallSnapAction.prepare(SnapdRootClient,
                                                 storeSnapsModel)]
        statusArea.updateContext("documentinfo",
                                 i18n("Available actions"), actions)
    }
}
