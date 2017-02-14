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
    id: departamentsRoot

    ListModel {
        id: departamentsModel

        Component.onCompleted: {
            var request = SnapStore.listDepartments()

            request.complete.connect(function () {
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
            })

            request.runAsync()
            //                    print(request)
        }
    }

    property string currentDepartamentSlug: ""

    ScrollView {
        anchors.fill: parent
        ListView {

            model: departamentsModel
            delegate: departamentDelegate
        }
    }

    Component {
        id: departamentDelegate
        Item {
            id: delegateRoot
            width: departamentsRoot.width
            height: 280
            property bool expanded: false
            Rectangle {
                anchors.fill: parent
                color: Utils.stringToColor(model.slug)
                opacity: 0.3
            }

            Text {
                id: sectionTitle
                x: delegateRoot.expanded ? 12 : delegateRoot.width / 2 - width / 2
                y: delegateRoot.expanded ? 12 : delegateRoot.height / 2 - height - 12

                text: model.name
                font.pointSize: 14
            }

            PlasmaComponents.Button {
                anchors.top: parent.verticalCenter
                anchors.topMargin: 12
                anchors.horizontalCenter: parent.horizontalCenter

                text: i18n("View")
                onClicked: {
                    delegateRoot.expanded = true
                    currentDepartamentSlug = model.slug
                    departamentSnapsLoader.sourceComponent = departamentSnaps
                }

                visible: !delegateRoot.expanded
            }

            Loader {
                id: departamentSnapsLoader
                anchors.fill: parent
                anchors.margins: 12
                anchors.topMargin: 38
            }

            PlasmaComponents.Button {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 6
                anchors.right: parent.right
                anchors.rightMargin: 24

                visible: delegateRoot.expanded
                text: i18n("Hide")
                onClicked: {
                    delegateRoot.expanded = false
                    departamentSnapsLoader.sourceComponent = undefined
                }
            }
        }
    }

    Component {
        id: departamentSnaps
        ListView {
            orientation: ListView.Horizontal
            model: SnapsModel {
                id: storeSnapsModel

                property var snaps
                onSelectedItemsChanged: {
                    refreshActions()
                }

                fetchSnapsFunc: function () {
                    var request = SnapStore.getDepartment(
                                currentDepartamentSlug)

                    request.runSync()

                    var pkgs = []
                    for (var i = 0; i < request.packagesCount(); i++) {
                        var pkg = request.package(i)
                        pkgs.push(pkg)
//                            storeSnapsModel.append(pkg)

                    }

                    //                        contentLoader.sourceComponent = snapsView
                    return pkgs

                }
                function refreshActions() {
                    var keys = Object.keys(selectedItems)
                    if (keys.length > 0) {
                        var actions = [InstallSnapAction.prepare(
                                           SnapdRootClient, storeSnapsModel)]
                        statusArea.updateContext("documentinfo",
                                                 i18n("Available actions"),
                                                 actions)
                    } else
                        statusArea.clearContext()
                }
                Component.onCompleted: {

                }
            }
            delegate: SnapElementDelegate {
                snap_name: title
                snap_version: version
                onSelectedChanged: {
                    if (selected)
                        storeSnapsModel.selectedItems[package_name] = "true"
                    else
                        delete storeSnapsModel.selectedItems[package_name]

                    storeSnapsModel.refreshActions()
                }
            }
        }
    }
}
