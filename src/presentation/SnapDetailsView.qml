import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import Snapd 1.0
import org.nx.softwarecenter 1.0

import "qrc:/actions/ActionFactory.js" as ActionFactory

import "parts" as Parts

Item {
    id: root
    width: 800
    height: 600

    SnapdClient {
        id: snapdClient
    }

    property var dismissCallback: function () {
        print("Dissmiss requested")
    }

    property string package_name: "minecraft-nsg"
    property var storeInfo
    property var localInfo

    function updateContext() {
        var actions = []
        print("package name:", localInfo.name)
        var enableAction = ActionFactory.prepareSimpleRequestAction(
                    textConstants.actionEnableTitle,
                    "package-installed-updated", function () {
                        return {
                            name: localInfo.name
                        }
                    }, function (target) {
                        var request = SnapdRootClient.enable(target.name)
                        return request
                    }, function () {}, function () {}, refesh)

        var disableAction = ActionFactory.prepareSimpleRequestAction(
                    textConstants.actionDisableTitle, "package-broken",
                    function () {
                        return {
                            name: localInfo.name
                        }
                    }, function (target) {
                        var request = SnapdRootClient.disable(target.name)
                        return request
                    }, function () {}, function () {}, refesh)

        var refreshAction = ActionFactory.prepareSimpleRequestAction(
                    textConstants.actionRefreshTitle, "package-upgrade",
                    function () {
                        return {
                            name: localInfo.name,
                            channel: localInfo.channel
                        }
                    }, function (target) {
                        var request = SnapdRootClient.refresh(target.name,
                                                              target.channel)

                        return request
                    }, function () {}, function () {}, refesh)

        var installAction = ActionFactory.prepareSimpleRequestAction(
                    textConstants.actionInstallTitle, "package-install",
                    function () {
                        return {
                            name: localInfo.name,
                            channel: localInfo.channel
                        }
                    }, function (target) {
                        var request = SnapdRootClient.install(target.name,
                                                              target.channel)
                        return request
                    }, function () {}, function () {}, refesh)

        var removeAction = ActionFactory.prepareSimpleRequestAction(
                    textConstants.actionRemoveTitle, "package-remove",
                    function () {
                        return {
                            name: localInfo.name,
                            channel: localInfo.channel
                        }
                    }, function (target) {
                        var request = SnapdRootClient.remove(target.name,
                                                             target.channel)
                        return request
                    }, function () {}, function () {}, refesh)

        if (localInfo) {
            if (localInfo.status >= 3) {
                actions.push(removeAction)

                if (localInfo.status >= 4)
                    actions.push(disableAction)
                else
                    actions.push(enableAction)

                if (storeInfo && storeInfo.revision > localInfo.revision) {
                    print("revisions", storeInfo.revision, localInfo.revision)
                    actions.push(refreshAction)
                }
            } else
                actions.push(installAction)
        }

        var returnAction = {
            icon: "draw-arrow-back",
            text: i18n("Return"),
            action: function () {
                dismissCallback()
            }
        }

        actions.push(returnAction)
        statusArea.updateContext("documentinfo",
                                 i18n("Available actions"), actions)
    }

    function fetchStoreInfo() {
        var storeRequest = SnapStore.getSnapDetails(package_name)
        storeRequest.complete.connect(function () {

            storeInfo = storeRequest.snapDetails()

            contentLoader.sourceComponent = detailsView
            updateContext()
        })

        storeRequest.runAsync()
    }

    function fetchLocalInfo() {
        // Ensure we are connected
        var connectRequest = snapdClient.connect()
        connectRequest.runSync()

        var localRequest = snapdClient.listOne(package_name)

        localRequest.complete.connect(function () {
            localInfo = localRequest.snap()

            contentLoader.sourceComponent = detailsView
            updateContext()
        })

        localRequest.runAsync()
    }

    function refesh() {
        fetchLocalInfo()
        fetchStoreInfo()
    }
    Component.onCompleted: refesh()

    ScrollView {
        anchors.fill: parent
        anchors.margins: 18
        Loader {
            id: contentLoader

            anchors.margins: 12
            sourceComponent: statusView
        }
    }

    Component {
        id: detailsView
        ColumnLayout {
            Layout.preferredWidth: 600

            RowLayout {
                Item {
                    Layout.rightMargin: 12
                    Layout.preferredHeight: 222
                    Layout.preferredWidth: 222

                    Rectangle {
                        anchors.fill: parent

                        clip: true
                        radius: 15

                        PlasmaCore.IconItem {
                            anchors.fill: parent
                            anchors.margins: 12

                            visible: (storeInfo === undefined)
                                     || (storeInfo.icon_url === undefined)
                            source: "package-available"
                        }

                        Image {
                            anchors.fill: parent
                            anchors.margins: 12
                            visible: storeInfo.icon_url !== undefined
                            source: visible
                                    && storeInfo.icon_url ? storeInfo.icon_url : ""
                        }
                    }
                }

                ColumnLayout {
                    Layout.rightMargin: 12
                    Layout.fillWidth: true

                    PlasmaComponents.Label {
                        id: snapTitle
                        Layout.topMargin: 16
                        Layout.minimumWidth: 300
                        Layout.maximumWidth: 300

                        text: storeInfo ? storeInfo.title : localInfo.name
                        wrapMode: Text.WordWrap
                        font.pointSize: 18
                    }

                    GridLayout {
                        Layout.minimumWidth: 300
                        Layout.maximumWidth: 300
                        Layout.alignment: Qt.AlignTop

                        Repeater {
                            model: storeInfo.keywords
                            delegate: PlasmaComponents.Label {
                                Layout.fillWidth: false
                                Layout.alignment: Qt.AlignLeft
                                text: storeInfo.keywords[index]
                                elide: "ElideRight"
                            }
                        }
                    }

                    Item {
                        Layout.rightMargin: 12
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }

                    RowLayout {
                        Layout.rightMargin: 12
                        Layout.bottomMargin: 12
                        Layout.fillWidth: true

                        PlasmaCore.IconItem {
                            Layout.preferredHeight: 24
                            Layout.preferredWidth: 24
                            source: "license"
                        }

                        PlasmaComponents.Label {
                            Layout.fillWidth: true
                            text: storeInfo.license
                        }
                    }
                }

                ColumnLayout {
                    Layout.topMargin: 18
                    Layout.alignment: Qt.AlignTop

                    PlasmaComponents.Label {
                        text: i18n("Developed by")
                    }

                    PlasmaComponents.Label {
                        text: storeInfo.publisher
                        wrapMode: Text.WordWrap
                    }

                    Parts.RatingStars {
                        id: snapStars
                        Layout.topMargin: 18
                        rating: storeInfo.ratings_average
                    }
                }
            }

            Flickable {
                Layout.columnSpan: 3
                Layout.margins: 12
                Layout.fillWidth: true
                Layout.preferredHeight: 500

                clip: true
                visible: root.storeInfo.screenshot_urls.length > 0

                contentWidth: screenshotsLayout.width
                contentHeight: screenshotsLayout.height

                RowLayout {
                    id: screenshotsLayout
                    Repeater {
                        model: root.storeInfo.screenshot_urls
                        delegate: Image {
                            source: root.storeInfo.screenshot_urls[index]
                        }
                    }
                }
            }

            PlasmaComponents.Label {
                id: snapDetails
                Layout.preferredWidth: 600
                Layout.maximumWidth: 600
                Layout.columnSpan: 3
                Layout.margins: 24
                text: root.storeInfo.description

                wrapMode: Text.Wrap
            }
        }
    }

    Component {
        id: statusView
        Item {
            PlasmaComponents.BusyIndicator {
                anchors.centerIn: parent
            }
        }
    }
}
