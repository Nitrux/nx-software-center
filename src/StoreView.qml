import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.nx.softwarecenter 1.0

Item {
    id: storeViewRoot

    objectName: "storeView"
    Rectangle {
        color: "lightblue"
        anchors.fill: parent
        opacity: 0.1
    }

    Loader {
        id: contentLoader
        anchors.fill: parent

        sourceComponent: storeSnapsModel.busy || storeSnapsModel.count == 0 ? searchView : snapsView
    }

    Component {
        id: searchView
        Item {
            PlasmaComponents.Label {
                id: messageText
                anchors.top: busyModelIndicator.bottom
                anchors.horizontalCenter: parent.horizontalCenter
                width: 400
                height: 100

                text: storeSnapsModel.errorMessage == "" ? storeSnapsModel.statusMessage : storeSnapsModel.errorMessage;

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

                source: storeSnapsModel.errorMessage == "" ? "face-laughing" : "face-uncertain"
                visible: !busyModelIndicator.visible
            }
        }
    }

    Component {
        id: snapsView
        ScrollView {
            Flickable {
                contentWidth: snapsList.width
                contentHeight: snapsList.height + 40
                anchors.margins: 20

                Flow {
                    anchors.centerIn: parent
                    id: snapsList
                    width: storeViewRoot.width - 20

                    Repeater {
                        model: storeSnapsModel
                        delegate: snaptElementDelegate
                    }
                }
            }
        }
    }


    Component {
        id: snaptElementDelegate
        Item {
            width: 192
            height: 192
            property bool busy: false

            Rectangle {
                anchors.fill: parent
                anchors.margins: 6
                color: "silver"
                opacity: 0.4
            }

            Action {
                id: installSnap
                iconName: "package-install"
                text: "&Install"
                onTriggered: {
                    print(text, model.name)
                    var request = SnapdRootClient.install(model.name, "stable")
                    function installCompleted() {
                        busy = false
                        print(request.errorString)
                        //storeSnapsModel.refresh()
                    }

                    request.finished.connect(installCompleted)
                    request.start()
                    busy = true
                }
            }

            Action {
                id: abortSnapAction
                iconName: "dialog-cancel"
                text: "&Abort"
                onTriggered: {
                    print(text)
                    busy = false
                }
            }

            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 6
                spacing: 0
                Item {
                    Layout.alignment: Qt.AlignRight
                    height: snap_status_icon.height
                    width: snap_status_icon.width
                    Layout.topMargin: 10
                    Layout.rightMargin: 12

                    PlasmaCore.IconItem {
                        id: snap_status_icon
                        source: ""
                        visible: !busy
                    }

                    PlasmaComponents.BusyIndicator {
                        id: busyIndicator
                        visible: busy
                        anchors.fill: snap_status_icon
                    }
                }

                PlasmaCore.IconItem {
                    id: snap_icon
                    Layout.preferredWidth: 64
                    Layout.preferredHeight: 64
                    Layout.alignment: Qt.AlignHCenter
                    source: model.icon ? model.icon : "package-available"
                }

                Text {
                    id: snap_pkg_name
                    text: name
                    elide: Text.ElideRight
                    Layout.topMargin: 14
                    Layout.leftMargin: 12
                    font.bold: true
                }

                Text {
                    id: snap_version
                    text: i18n("Version: ") + version
                    Layout.leftMargin: 12
                    font.italic: true
                }
                Text {
                    id: snap_installed_size
                    property string sizeString: {
                        var value = model.downaloadSize
                        var unit = "bytes"
                        if (value > 1024) {
                            value = value / 1024
                            unit = "KiB"
                        }

                        if (value > 1024) {
                            value = value / 1024
                            unit = "MiB"
                        }

                        if (value > 1024) {
                            value = value / 1024
                            unit = "GiB"
                        }

                        return "" + Math.round(value, 2) + " " + unit
                    }
                    text: model.downaloadSize ? sizeString : i18n("Unknown size")
                    Layout.leftMargin: 12
                    Layout.fillHeight: true
                }
            }

            MouseArea {
                id: snapElementArea
                anchors.fill: parent

                preventStealing: false
                propagateComposedEvents: true
                hoverEnabled: true

                PlasmaComponents.ButtonColumn {
                    id: snapActions
                    opacity: snapElementArea.containsMouse ? 1 : 0
                    exclusive: false
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.margins: 6
                    spacing: 2

                    PlasmaComponents.Button {
                        action: installSnap
                        visible: !busy
                    }

                    PlasmaComponents.Button {
                        action: abortSnapAction
                        visible: busy
                    }

                    Behavior on opacity {
                        PropertyAnimation {
                        }
                    }
                }
            }
        }
    }

    ListModel {
        id: storeSnapsModel

        property bool busy: false
        property string query: ""
        property string statusMessage: i18n("Type what are you looking for ...")
        property string errorMessage: ""

        Component.onCompleted: refresh()

        function refresh() {
            if (searchField.text !== query)
                query  = searchField.text
            else
                return


            busy = true
            statusMessage = i18n("Lonking for snaps like: \"") + query + "\""
            var request = snapdClient.find(0, query)

            function findCompleted() {
                storeSnapsModel.clear()

                if (request.snapCount == 0)
                    statusMessage = i18n("No snaps where found with text \"") +  query + "\" try something else."

                print(request.errorString)
                if(request.error)
                    errorMessage = i18n("There was an error while procesing your request. Please check your internet connection and try again.")
                else
                    errorMessage = ""

                console.log("Available:")
                for (var i = 0; i < request.snapCount; i++) {
                    var snap = request.snap(i)
                    console.log(snap.name, snap.installDate)
                    storeSnapsModel.append(snap)
                }
                busy = false
            }

            request.complete.connect(findCompleted)
            request.runAsync()
        }
    }

    Connections {
        target: searchField
        onEditingFinished: storeSnapsModel.refresh()
    }
}
