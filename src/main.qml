import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.2 as QtControls
import QtQuick.Controls.Styles.Plasma 2.0 as Styles


import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import QtQuick.Controls.Styles.Plasma 2.0 as Styles
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.nx.softwarecenter 1.0 as SoftwareCenter

//import Snapd 1.0


ApplicationWindow {
    visible: true
    width: 600
    height: 400
    title: qsTr("NX Software Center")


//    SnapdClient {
//        id: client

//        Component.onCompleted: {


//            var installRequest = client.install("bas", "stable")
//            print(installRequest)

//            var request = client.list();

//            request.onIsFinishedChanged = function (){ print ("requset.isFinished", requset.isFinished)}
//            print("request.isFinished",request.isFinished,  "request.snapCount", request.snapCount)
//            for (var i = 0; i < request["snapCount"]; i++) {
//                print(request.snap(i))
//            }

//        }
//    }


    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Layout.leftMargin: 12
            Layout.rightMargin: 12
            Layout.topMargin: 6
            Layout.bottomMargin: 6
            Layout.fillWidth: true

            spacing: 8
            PlasmaComponents.Button {
                iconName: "go-home"
                checked: content.currentItem
                         && content.currentItem.objectName == "homeView"
                onClicked: {
                    content.clear()
                    content.push(homeView)
                }
            }
            PlasmaComponents.Button {
                iconName: "application"
            }
            PlasmaComponents.Button {
                iconName: "favorites"
                checked: content.currentItem
                         && content.currentItem.objectName == "changesView"
                onClicked: {
                    content.clear()
                    content.push(changesView)
                }
            }
            PlasmaComponents.Button {
                iconName: "edit-download"
                checked: content.currentItem
                         && content.currentItem.objectName == "workView"
                onClicked: {
                    content.clear()
                    content.push(workView)
                }
            }

            PlasmaComponents.TextField {
                id: searchField
                Layout.fillWidth: true
                Layout.leftMargin: 100
                Layout.rightMargin: 100
                placeholderText: "Search"
            }

            PlasmaComponents.Button {
                Layout.alignment: Qt.AlignRight
                iconName: "configure"
            }
        }

        StackView {
            id: content
            Layout.fillWidth: true
            Layout.fillHeight: true

            initialItem: homeView
        }
    }

    Component {
        id: changesView
        ChangesView {
            objectName: "changesView"
        }
    }
    Component {
        id: homeView
        Flow {
            padding: 12
            spacing: 24
            objectName: "homeView"


            Connections {
                target: searchField
                onEditingFinished: {
                    print("Setting filter on local snaps: ", searchField.text)
                    print("Not implemente yet")
                }
            }

            Repeater {
                model: SoftwareCenter.LocalIndex {
                    id: localSnaps
                }

                delegate: SnapItemDelegate {
                }
            }
        }
    }

    Component {
        id: workView
        Item {
            objectName: "workView"
            Rectangle {
                color: "lightblue"
                opacity: 0.3
                anchors.fill: parent
            }

            Flow {
                padding: 12
                spacing: 24
                anchors.fill: parent
                Connections {
                    target: searchField
                    onEditingFinished: {

                        remoteSnaps.setTextFilter(searchField.text)
                    }
                }

                Repeater {
                    model: SoftwareCenter.StoreIndex {
                        id: remoteSnaps
                    }

                    delegate: Item {
                        height: 152
                        width: 152
                        Rectangle {
                            anchors.fill: parent
                            color: "lightblue"
                            opacity: 0.3
                        }

                        GridLayout {
                            anchors.fill: parent
                            columns: 2
                            rowSpacing: 0
                            columnSpacing: 0

                            PlasmaCore.IconItem {
                                Layout.columnSpan: 2
                                Layout.alignment: Qt.AlignRight
                                Layout.topMargin: 6
                                Layout.rightMargin: 6
                                Layout.bottomMargin: 0
                                source: {
                                    if (_status == "active")
                                        return "emblem-success"
                                }

                                Layout.preferredHeight: 24
                                Layout.preferredWidth: 24
                            }

                            Image {
                                Layout.preferredHeight: 64
                                Layout.preferredWidth: 64

                                Layout.alignment: Qt.AlignHCenter
                                Layout.columnSpan: 2
                                source: _icon != "" ? _icon : "package-x-generic"
                            }

                            ColumnLayout {
                                Layout.leftMargin: 4
                                spacing: 0
                                PlasmaComponents.Label {
                                    text: _name
                                    font.bold: true
                                    font.italic: true
                                    font.pointSize: 8
                                }
                                PlasmaComponents.Label {
                                    text: "Version: " + _version
                                    font.pointSize: 8
                                }

                                PlasmaComponents.Label {
                                    text: "Size: " + (_download_size / 1024*1024)
                                    font.pointSize: 8
                                }
                            }

                            PlasmaComponents.Button {
                                text: "Install"
                                Layout.preferredHeight: 28
                                Layout.preferredWidth: 64

                                style: Styles.ButtonStyle {
                                    label: Item {
                                        Label {
                                            anchors.centerIn: parent
                                            text: control.text
                                            font.pointSize: 8
                                        }
                                    }
                                }
                                onClicked: SoftwareCenter.Snaps.install(_name)
                            }
                        }
                    }
                }
            }
        }
    }
}
