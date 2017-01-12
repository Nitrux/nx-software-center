import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

Item {
    id: homeViewRoot
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
                    model: testSnapsModel
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
            property bool busy: false;

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
                onTriggered: {print(text); busy = true}
            }

            Action {
                id: disableSnap
                iconName: "package-broken"
                text: "&Disable"
                onTriggered: {print(text); busy = true}
            }

            Action {
                id: removeSnap
                iconName: "package-remove"
                text: "&Remove"
                onTriggered: {print(text); busy = true}
            }

            Action {
                id: refreshSnap
                iconName: "package-upgrade"
                text: "Re&fresh"
                onTriggered: {print(text); busy = true}
            }

            Action {
                id: abortSnapAction
                iconName: "dialog-cancel"
                text: "&Abort"
                onTriggered: {print(text); busy = false}
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
                        source: snap_enabled ? "package-installed-updated" : "package-broken"
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
                    source: icon_url != null ? icon_url : "package-available"
                }

                Text {
                    id: snap_pkg_name
                    text: package_name
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
                        var value = installed_size
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
                    text: installed_size ? sizeString : i18n("Unknown size")
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
                onClicked: mouse.accepted = falses

                PlasmaComponents.ButtonColumn {
                    id: snapActions
                    opacity: snapElementArea.containsMouse ? 1 : 0
                    exclusive: false
                    anchors.bottom: parent.bottom
                    anchors.right: parent.right
                    anchors.margins: 6
                    spacing: 2

                    PlasmaComponents.Button {
                        action: snap_enabled ? disableSnap : enableSnap
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

    // Sample data for testing/development proposes
    ListModel {
        id: testSnapsModel

        ListElement {
            //            architecture: ["amd64"]
            //            binary_filesize: null
            channel: "stable"
            confinement: "strict"
            content: "application"
            description: "This is a test snap"
            icon_url: "firefox-esr"
            last_updated: "2016-06-03T12:01:15.435821Z"
            origin: "testuser"
            package_name: "Mozilla Firefox"
            //            prices: undefined
            publisher: "TestUser"
            ratings_average: 0.0
            revision: 1
            snap_id: "xxxxxxxxxxxxxxxxxxxxxxxxfoobar25"
            summary: "This is a test snap"
            version: "48.0"
            snap_enabled: true
            installed_size: 101048576
        }

        ListElement {
            //            architecture: ["amd64"]
            //            binary_filesize: null
            channel: "stable"
            confinement: "strict"
            content: "application"
            description: "This is a test snap"
            icon_url: "clementine"
            last_updated: "2016-06-03T12:01:15.435821Z"
            origin: "testuser"
            package_name: "Clementine"
            //            prices: undefined
            publisher: "TestUser"
            ratings_average: 0.0
            revision: 1
            snap_id: "xxxxxxxxxxxxxxxxxxxxxxxxfoobar25"
            summary: "This is a test snap"
            version: "2.5"
            snap_enabled: true
            installed_size: 200000000003
        }
        ListElement {
            //            architecture: ["amd64"]
            //            binary_filesize: null
            channel: "stable"
            confinement: "strict"
            content: "application"
            description: "This is a test snap"
            //            icon_url: undefined
            last_updated: "2016-06-03T12:01:15.435821Z"
            origin: "testuser"
            package_name: "foobar25"
            //            prices: undefined
            publisher: "TestUser"
            ratings_average: 0.0
            revision: 1
            snap_id: "xxxxxxxxxxxxxxxxxxxxxxxxfoobar25"
            summary: "This is a test snap"
            version: "2.5"
        }
        ListElement {
            //            architecture: ["amd64"]
            //            binary_filesize: null
            channel: "stable"
            confinement: "strict"
            content: "application"
            description: "This is a test snap"
            //            icon_url: undefined
            last_updated: "2016-06-03T12:01:15.435821Z"
            origin: "testuser"
            package_name: "foobar25"
            //            prices: undefined
            publisher: "TestUser"
            ratings_average: 0.0
            revision: 1
            snap_id: "xxxxxxxxxxxxxxxxxxxxxxxxfoobar25"
            summary: "This is a test snap"
            version: "2.5"
            snap_enabled: false
        }
        ListElement {
            //            architecture: ["amd64"]
            //            binary_filesize: null
            channel: "stable"
            confinement: "strict"
            content: "application"
            description: "This is a test snap"
            //            icon_url: undefined
            last_updated: "2016-06-03T12:01:15.435821Z"
            origin: "testuser"
            package_name: "foobar25"
            //            prices: undefined
            publisher: "TestUser"
            ratings_average: 0.0
            revision: 1
            snap_id: "xxxxxxxxxxxxxxxxxxxxxxxxfoobar25"
            summary: "This is a test snap"
            version: "2.5"
            snap_enabled: false
            snap_installed_size: 200003
        }
        ListElement {
            //            architecture: ["amd64"]
            //            binary_filesize: null
            channel: "stable"
            confinement: "strict"
            content: "application"
            description: "This is a test snap"
            //            icon_url: undefined
            last_updated: "2016-06-03T12:01:15.435821Z"
            origin: "testuser"
            package_name: "foobar25"
            //            prices: undefined
            publisher: "TestUser"
            ratings_average: 0.0
            revision: 1
            snap_id: "xxxxxxxxxxxxxxxxxxxxxxxxfoobar25"
            summary: "This is a test snap"
            version: "2.5"
            snap_enabled: true
        }
    }
}
