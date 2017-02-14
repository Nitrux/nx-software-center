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
                    delegateRoot.expanded = true;
                    currentDepartamentSlug = model.slug
                    departamentSnapsLoader.sourceComponent = departamentSnaps
                }

                visible: !delegateRoot.expanded
            }

            Loader {
                id: departamentSnapsLoader
                anchors.fill: parent
                anchors.margins: 12
                anchors.topMargin: 32
            }

            PlasmaComponents.Button {
                anchors.bottom: parent.bottom
                anchors.bottomMargin: 6
                anchors.right: parent.right
                anchors.rightMargin: 24

                visible: delegateRoot.expanded
                text: i18n("Hide")
                onClicked: {
                    delegateRoot.expanded = false;
                    departamentSnapsLoader.sourceComponent = undefined
                }
            }
        }
    }

    Component {
        id: departamentSnaps
        ListView {
            orientation: ListView.Horizontal
            model: ListModel {
                id: storeSnapsModel
                Component.onCompleted: {
                    var request = SnapStore.getDepartment(currentDepartamentSlug)

                    request.complete.connect(function () {
                        for (var i = 0; i < request.packagesCount(); i++) {
                            var pkg = request.package(i)
                            storeSnapsModel.append(pkg)
                        }

//                        contentLoader.sourceComponent = snapsView
                    })
                    request.runAsync()
                }
            }
            delegate: SnapElementDelegate {
                snap_name: title
                snap_version: version
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
