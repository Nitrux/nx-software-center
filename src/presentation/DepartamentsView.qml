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

    property ListModel departamentsModel;

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
            height: 260
            property bool expanded: false
            Rectangle {
                anchors.fill: parent
                color: Utils.stringToColor(model.slug)
                opacity: 0.3
            }

            Text {
                id: sectionTitle
                x: delegateRoot.width / 2 - width / 2
                y: delegateRoot.height / 2 - height - 12

                text: model.name
                font.pointSize: 14
            }

            PlasmaComponents.Button {
                anchors.top: parent.verticalCenter
                anchors.topMargin: 12
                anchors.horizontalCenter: parent.horizontalCenter

                text: i18n("View")
                onClicked: {
                    currentDepartamentSlug = model.slug
                    departamentSnapsLoader.sourceComponent = departamentSnaps
                }
            }

            Loader {
                id: departamentSnapsLoader
                anchors.fill: parent
            }

            Component.onCompleted: {


                //                            print("completed", model.slug)
                //                            var request2 = SnapStore.getDepartment(model.slug);
                //                            var f = function () {
                //                                print("loading details finished")
                //                            }
                //                            request2.finished.connect(f)
                //                            request2.start()
                //                            var getDepartmentRequest = SnapStore.getDepartment(model.slug)

                //                            getDepartmentRequest.finished.connect(function () {
                //                                print("highlight: ", model.slug, getDepartmentRequest.highlightsCount())

                //                                for (var i = 0; i < getDepartmentRequest.highlightsCount(); i++) {
                //                                    var highlight = getDepartmentRequest.highlight(i)
                //                                    highlights.push(highlight);

                //                                    print(highlight.name, highlight.description)
                //                                    var packages = highlight.packages

                //                                    for (var i = 0; i < packages.length; i++) {
                //                                        print(packages[i].title, packages[i].publisher)
                //                                    }
                //                                }

                //                                print("packagesCount", request.packagesCount())
                //                                for (var i = 0; i < request.packagesCount(); i++) {
                //                                    var pkg = request.package(i)
                //                                    print(pkg.title, pkg.publisher)
                //                                }
                //                            })

                //                            getDepartmentRequest.start()
            }
        }
    }

    Component {
        id: departamentSnaps
        ListView {

            model: ListModel {
                Component.onCompleted: {
                    var boop = SnapStore.getDepartment("universal-accessaccessibility")

                    var onFinished = function () {
                        print ("hi!");
                        for (var i = 0; i < request.packagesCount(); i++) {
                            var pkg = request.package(i)
                            storeSnapsModel.push(pkg)
                        }

                        contentLoader.sourceComponent = snapsView
                        print("at last !")
                    }


                    boop.complete.connect(onFinished)
                    print(boop.complete)
                    boop.runAsync()
                }
            }
            delegate: SnapElementDelegate {
                snap_name: name
                snap_version: version
                snap_size: downaloadSize
                onSelectedChanged: {
                    if (selected)
                        storeSnapsModel.selectedItems[name] = "true"
                    else
                        delete storeSnapsModel.selectedItems[name]
                }
            }
        }
    }
}
