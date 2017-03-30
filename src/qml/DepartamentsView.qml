import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import Snapd 1.0

import org.nx.softwarecenter 1.0
import "qrc:/scripts/Utils.js" as UtilsJs

import "parts" as Parts
import "interactors" as Interactors

Parts.View {
    id: departamentsRoot

    property var p: QtObject {
        signal refreshContent();
    }

    function refreshContent() {
        // No need to refresh departaments, this rarely changes
        // listDepartamentsInteractor.run()

        p.refreshContent()
    }

    Interactors.ListDepartamentsInteractor {
        id: listDepartamentsInteractor

        onLoading: showLoadingScreen(
                       i18n("Listing departaments, please wait ..."))
        onError: showError(message)
        onComplete: departamentsRoot.sourceComponent = component_departamentsList

        Component.onCompleted: listDepartamentsInteractor.run()
    }

    Component {
        id: component_departamentsList
        Item {
            Parts.HorizontalListView {
                id: departamentsListView
                anchors {
                    top: parent.top
                    left: parent.left
                    leftMargin: 12
                    right: parent.right
                    rightMargin: 6
                }
                height: 200

                focus: true
                property int viewIndex: 0

                spacing: 12
                orientation: ListView.Horizontal
                highlightFollowsCurrentItem: true
                highlight: Item {
                }
                model: listDepartamentsInteractor.departamentsListModel
                delegate: Parts.Card {
                    width: 160
                    height: 200
                    clip: true

                    color: index == departamentsListView.currentIndex ? "#aae3ff" : "white"
                    Image {
                        width: 160
                        height: 160
                        source: "file:///usr/share/nx_software_center/departaments_background/"
                                + model.slug + ".png"
                        fillMode: Image.PreserveAspectCrop
                        opacity: index == departamentsListView.currentIndex ? 1 : 0.7
                    }

                    Text {
                        anchors.left: parent.left
                        anchors.bottom: parent.bottom
                        anchors.margins: 12
                        text: model.name
                    }
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            departamentsListView.currentIndex = index
                            departamentSnaps.departamentSlug = model.slug
                        }
                    }
                }
            }

            DepartamentSnapsView {
                id: departamentSnaps
                anchors {
                    left: parent.left
                    right: parent.right
                    top: split.bottom
                    topMargin: split.height * -1
                    bottom: parent.bottom
                }

                departamentSlug: listDepartamentsInteractor.departaments.length
                                                  > 0 ? listDepartamentsInteractor.departaments[0].slug : ""

                Connections {
                    target: p
                    onRefreshContent: departamentSnaps.updateActions()
                }
            }

            Rectangle {
                id: split
                anchors.top: departamentsListView.bottom
                anchors.topMargin: 12
                anchors.left: parent.left
                anchors.right: parent.right
                height: 6

                gradient: Gradient {
                    GradientStop {
                        position: 0.0
                        color: "#AFAFB1"
                    }
                    GradientStop {
                        position: 1.0
                        color: "transparent"
                    }
                }
            }
        }
    }
}
