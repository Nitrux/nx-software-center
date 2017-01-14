import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.nx.softwarecenter 1.0

Item {
    id: homeViewRoot

    objectName: "homeView"
    Rectangle {
        color: "lightblue"
        anchors.fill: parent
        opacity: 0.1
    }

    ScrollView {
        anchors.fill: parent

        Flickable {
            contentWidth: snapsList.width
            contentHeight: snapsList.height
            anchors.margins: 20

            Flow {
                anchors.centerIn: parent
                id: snapsList
                width: homeViewRoot.width - 20

                Repeater {
                    model: installedSnapsModel
                    delegate: snaptElementDelegate
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
            property var request

            Rectangle {
                anchors.fill: parent
                anchors.margins: 6
                color: "silver"
                opacity: 0.4
            }

            Action {
                id: enableSnap
                iconName: "package-installed-updated"
                text: "&Enable"
                onTriggered: {
                    print(text, model.name)
                    request = SnapdRootClient.enable(model.name);
                    function enableCompleted() {
                        busy = false;
                        print(request.errorString)
                        installedSnapsModel.refresh()
                    }

                    request.finished.connect(enableCompleted)
                    request.start()
                    busy = true
                }
            }

            Action {
                id: disableSnap
                iconName: "package-broken"
                text: "&Disable"
                onTriggered: {
                    print(text, model.name)
                    request = SnapdRootClient.disable(model.name);
                    function disableCompleted() {
                        busy = false;
                        print(request.errorString)
                        installedSnapsModel.refresh()
                    }
                    request.finished.connect(disableCompleted)
                    request.start()
                    busy = true
                }
            }

            Action {
                id: removeSnap
                iconName: "package-remove"
                text: "&Remove"
                onTriggered: {
                    print(text, model.name)
                    request = SnapdRootClient.remove(model.name);
                    function removeCompleted() {
                        busy = false;
                        print(request.errorString)
                        installedSnapsModel.refresh()
                    }
                    request.finished.connect(removeCompleted)
                    request.start()
                    busy = true
                }
            }

            Action {
                id: refreshSnap
                iconName: "package-upgrade"
                text: "Re&fresh"
                onTriggered: {
                    print(text, model.name)
                    request = SnapdRootClient.refresh(model.name, model.channel)

                    function requestCompleted() {

                        busy = false;
                        print(request.errorString)
                        installedSnapsModel.refresh()
                    }
                    request.finished.connect( requestCompleted )
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
                    request.kill()
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
                        source: model.status == 4 ? "package-installed-updated" : "package-broken"
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
                    maximumLineCount: 1
                    wrapMode: Text.WrapAnywhere
                    Layout.topMargin: 14
                    Layout.leftMargin: 12
                    Layout.maximumWidth: 140
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
                        var value = installedSize
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
                    text: installedSize ? sizeString : i18n("Unknown size")
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
                        action: status == 4 ? disableSnap : enableSnap
                        visible: !busy
                    }
                    PlasmaComponents.Button {
                        action: refreshSnap
                        visible: !busy
                    }
                    PlasmaComponents.Button {
                        action: removeSnap
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
        id: installedSnapsModel

        Component.onCompleted: refresh()

        function refresh() {
            installedSnapsModel.clear()
            var listRequest = snapdClient.list()
            listRequest.runSync()

//            console.log("Installed:")
            for (var i = 0; i < listRequest.snapCount; i++) {
                var snap = listRequest.snap(i)
//                console.log(snap.name, snap.installDate)
                installedSnapsModel.append(snap)
            }
        }
    }

    Connections {
        target: snapdClient
        onConnected: installedSnapsModel.refresh()
    }
}
