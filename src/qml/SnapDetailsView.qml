import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import "parts" as Parts
import "interactors" as Interactors

Item {
    id: root

    objectName: "snapDetailsView"
    signal refresh
    signal dismiss

    property var snapLocalInfo: undefined
    property var snapStoreInfo: undefined
    onSnapLocalInfoChanged: {
        for (var k in snapLocalInfo) {
            print("snapLocalInfo: ",k, snapLocalInfo[k])
        }
    }
    onSnapStoreInfoChanged: {
        for (var k in snapStoreInfo) {
            print("snapStoreInfo:",k, snapStoreInfo[k])
        }
    }

    property string name: snapStoreInfo ? snapStoreInfo.title : snapLocalInfo.name
    property string description: snapStoreInfo ? snapStoreInfo.description : snapLocalInfo.description
    property var keywords: snapStoreInfo ? snapStoreInfo.keywords : []
    property string license: snapStoreInfo ? snapStoreInfo.license : ""
    property string publisher: snapStoreInfo ? snapStoreInfo.publisher : ""
    property var ratings_average: snapStoreInfo ? snapStoreInfo.ratings_average : -1
    property var screenshot_urls: snapStoreInfo ? snapStoreInfo.screenshot_urls : []

    property var icon: snapStoreInfo ? snapStoreInfo.icon_url : undefined

    property var snapInfo

    function updateContext() {
        var actions = []
        var enableAction = {
            text: textConstants.actionEnableTitle,
            icon: "package-installed-updated",
            action: function () {
                enableSnapInteractor.targets = [package_name]
                enableSnapInteractor.finished.connect(refresh)

                enableSnapInteractor.start()
            }
        }

        var disableAction = {
            icon: "package-broken",
            text: textConstants.actionDisableTitle,
            action: function () {
                disableSnapInteractor.targets = [package_name]
                disableSnapInteractor.finished.connect(refresh)

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
                refreshSnapInteractor.finished.connect(refresh)

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
                installSnapInteractor.finished.connect(refresh)

                installSnapInteractor.start()
            }
        }

        var removeAction = {
            icon: "package-remove",
            text: textConstants.actionRemoveTitle,
            action: function () {
                removeSnapInteractor.targets = [package_name]
                removeSnapInteractor.finished.connect(refresh)

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
            action: dismiss()
        }

        actions.push(returnAction)
        statusArea.updateContext("documentinfo",
                                 i18n("Available actions"), actions)
    }

    ScrollView {
        anchors.fill: parent
        anchors.margins: 18

        Parts.Card {
            color: "#FCFCFC"
            width: 750
            height: snapdDetailsLayout.height
            x: 6
            y: 6

            ColumnLayout {
                id: snapdDetailsLayout
                width: parent.width
                implicitWidth: parent.width

                RowLayout {
                    Layout.fillWidth: true
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
                                visible: icon_url !== undefined
                                source: icon_urls
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.rightMargin: 12
                        Layout.fillWidth: true

                        PlasmaComponents.Label {
                            id: snapTitle
                            Layout.topMargin: 16
                            Layout.fillWidth: true

                            text: name
                            wrapMode: Text.WordWrap
                            font.pointSize: 18
                        }

                        Flow {
    //                        columns: 3
                            Layout.fillWidth: true

                            //                        Layout.alignment: Qt.AlignTop
                            Repeater {
                                model: keywords
                                delegate: PlasmaComponents.Label {
    //                                Layout.fillWidth: false
    //                                Layout.alignment: Qt.AlignLeft
                                    text: keywords[index]
                                    elide: "ElideRight"
    //                                Layout.maximumWidth: 100
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

                            visible: license != ""

                            PlasmaCore.IconItem {
                                Layout.preferredHeight: 24
                                Layout.preferredWidth: 24
                                source: "license"
                            }

                            PlasmaComponents.Label {
                                Layout.fillWidth: true
                                text: license
                            }
                        }
                    }

                    ColumnLayout {
                        Layout.topMargin: 18
                        Layout.alignment: Qt.AlignTop

                        visible: publisher != ""
                        PlasmaComponents.Label {
                            text: i18n("Developed by")
                        }

                        PlasmaComponents.Label {
                            text: publisher
                            wrapMode: Text.WordWrap
                        }

                        Parts.RatingStars {
                            id: snapStars
                            Layout.topMargin: 18
                            rating: ratings_average
                            visible: ratings_average >= 0
                        }
                    }
                }

                Image {
                    id: screenshots
                    Layout.fillWidth: true
                    Layout.preferredHeight: 500

                    clip: true
                    visible: screenshot_urls.length > 0

                    source: screenshot_urls[0]
                    fillMode: Image.PreserveAspectFit

                    RowLayout {
                        anchors.horizontalCenter: parent.horizontalCenter
                        anchors.bottom: parent.bottom
                        anchors.bottomMargin: 12

                        Repeater {
                            model: screenshot_urls.length
                            delegate: Rectangle {
                                height: 15
                                width: 15
                                radius: 15

                                color: screenshots.source
                                       == screenshot_urls[index] ? "blue" : "white"
                                border.color: "black"
                                border.width: 2

                                MouseArea {
                                    anchors.fill: parent
                                    onClicked: screenshots.source
                                               = screenshot_urls[index]
                                }
                            }
                        }
                    }
                }

                PlasmaComponents.Label {
                    id: snapDetails
                    text: description
                    wrapMode: Text.Wrap
                    Layout.fillWidth: true
                }
            }

        }

    }
}
