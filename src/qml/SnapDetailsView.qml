import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import Snapd 1.0
import org.nx.softwarecenter 1.0

import "qrc:/actions/ActionFactory.js" as ActionFactory

import "parts" as Parts
import "interactors" as Interactors

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
        var enableAction = {
            text: textConstants.actionEnableTitle,
            icon: "package-installed-updated",
            action: function () {
                enableSnapInteractor.targets = [package_name]
                enableSnapInteractor.finished.connect(refesh)

                enableSnapInteractor.start()
            }
        }

        var disableAction = {
            icon: "package-broken",
            text: textConstants.actionDisableTitle,
            action: function () {
                disableSnapInteractor.targets = [package_name]
                disableSnapInteractor.finished.connect(refesh)

                disableSnapInteractor.start()
            }
        }

        var refreshAction = {
            icon: "package-upgrade",
            text: textConstants.actionRefreshTitle,
            action: function () {
                refreshSnapInteractor.targets = [{
                                                     name: package_name,
                                                     channel: localInfo.channel
                                                 }]
                refreshSnapInteractor.finished.connect(refesh)

                refreshSnapInteractor.start()
            }
        }
        var installAction = {
            icon: "package-install",
            text: textConstants.actionInstallTitle,
            action: function () {
                installSnapInteractor.targets = [{
                                                     name: package_name,
                                                     channel: localInfo.channel
                                                 }]
                installSnapInteractor.finished.connect(refesh)

                installSnapInteractor.start()
            }
        }

        var removeAction = {
            icon: "package-remove",
            text: textConstants.actionRemoveTitle,
            action: function () {
                removeSnapInteractor.targets = [package_name]
                removeSnapInteractor.finished.connect(refesh)

                removeSnapInteractor.start()
            }
        }

        if (localInfo) {
            if (localInfo.status >= 3) {
                actions.push(removeAction)

                if (localInfo.status >= 4)
                    actions.push(disableAction)
                else
                    actions.push(enableAction)

                if (storeInfo && storeInfo.revision > localInfo.revision) {
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
                        columns: 3
                        Layout.minimumWidth: 300
                        Layout.maximumWidth: 300

                        //                        Layout.alignment: Qt.AlignTop
                        Repeater {
                            model: storeInfo.keywords
                            delegate: PlasmaComponents.Label {
                                Layout.fillWidth: false
                                Layout.alignment: Qt.AlignLeft
                                text: storeInfo.keywords[index]
                                elide: "ElideRight"
                                Layout.maximumWidth: 100
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

            Image {
                id: screenshots
                Layout.columnSpan: 3
                Layout.margins: 12
                Layout.fillWidth: true
                Layout.preferredHeight: 500

                clip: true
                visible: root.storeInfo.screenshot_urls.length > 0

                source: root.storeInfo.screenshot_urls[0]
                fillMode: Image.PreserveAspectFit

                RowLayout {
                    anchors.horizontalCenter: parent.horizontalCenter
                    anchors.bottom: parent.bottom
                    anchors.bottomMargin: 12

                    Repeater {
                        model: root.storeInfo.screenshot_urls.length
                        delegate: Rectangle {
                            height: 15
                            width: 15
                            radius: 15

                            color: screenshots.source
                                   == root.storeInfo.screenshot_urls[index] ? "blue" : "white"
                            border.color: "black"
                            border.width: 2

                            MouseArea {
                                anchors.fill: parent
                                onClicked: screenshots.source
                                           = root.storeInfo.screenshot_urls[index]
                            }
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
