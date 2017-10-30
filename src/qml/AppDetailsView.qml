import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import "parts" as Parts
import "interactors" as Interactors
import "scripts/Utils.js" as Utils

Item {
    id: root

    objectName: "appImageDetailsView"
    signal refresh
    signal dismiss

    property var app;

    function updateContext() {
        var actions = []
        var getAction = {
            text: textConstants.actionEnableTitle,
            icon: "package-installed-updated",
            action: function () {
                console.log("Working on it");
            }
        }

        actions.push(getAction)

        var returnAction = {
            icon: "draw-arrow-back",
            text: i18n("Return"),
            action: dismiss
        }

        actions.push(returnAction)
        statusArea.updateContext("emblem-info",
                                 i18n("Available actions"), actions)
    }

    Parts.Card {
        color: theme.viewBackgroundColor
        width: cardContent.width + 12
        height: cardContent.height + 12
        anchors.top: parent.top
        anchors.topMargin: 12
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 4

        ColumnLayout {
            id: cardContent
            width: root.width - 60 // 12 poinst of left margin
            height: parent.height - 12
            anchors.top: parent.top
            anchors.topMargin: 6
            anchors.left: parent.left
            anchors.leftMargin: 6
            anchors.bottomMargin: 12

            RowLayout {
                id: sectionHeader
                spacing: 16

                Layout.maximumHeight: 222
                Layout.fillWidth: true

                Parts.SnapIcon {
                    Layout.fillHeight: true
                    Layout.fillWidth: false
                    Layout.preferredHeight: 222
                    Layout.preferredWidth: 222

                    source: app['icon_link']
                }

                ColumnLayout {
                    Layout.fillHeight: true
                    Layout.fillWidth: true
                    Layout.minimumHeight: 222
                    Layout.maximumHeight: 222

                    spacing: 0

                    PlasmaComponents.Label {
                        id: snapTitle
                        Layout.topMargin: 16
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignTop

                        text: Utils.jsUcfirst(app['name'])
                        wrapMode: Text.WordWrap
                        font.pointSize: 18
                    }

                    Flow {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignTop

                        spacing: 10
                        opacity: 0.7

                        Repeater {
                            model: app['categories']
                            delegate: PlasmaComponents.Label {
                                text: snap.keywords[index]
                                elide: "ElideRight"
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 200
                        Layout.bottomMargin: 18

                        PlasmaCore.IconItem {
                            Layout.alignment: Qt.AlignBottom
                            Layout.preferredHeight: 24
                            Layout.preferredWidth: 24
                            source: "license"

                            visible: app['license'] != ""
                        }

                        PlasmaComponents.Label {
                            Layout.alignment: Qt.AlignBottom
                            Layout.fillWidth: true
                            text: app['license']

                            visible: app['license'] != ""
                        }
                    }
                }

                ColumnLayout {
                    Layout.maximumHeight: 222
                    Layout.maximumWidth: 300
                    PlasmaComponents.Label {
                        Layout.fillWidth: true
                        Layout.topMargin: 24
                        visible: app['authors'] != ""
                        text: i18n("Developed by")

                        Layout.maximumHeight: 20
                    }

                    PlasmaComponents.Label {
                        Layout.fillWidth: true
                        visible: app['authors'] != ""
                        text: app['authors']
                        wrapMode: Text.WordWrap

                        Layout.alignment: Qt.AlignTop
                    }

                    Item {
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.preferredHeight: 140
                    }

                    Parts.RatingStars {
                        id: appStars
                        Layout.fillWidth: true
                        rating: app['ratings_average']
                        visible: app['ratings_average'] >= 0

                        Layout.bottomMargin: 16
                    }
                }
            }

            Flickable {
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.bottomMargin: 24
                clip: true

                contentWidth: width
                contentHeight: sectionCaptures.visible ? sectionCaptures.height
                                                         + appDetails.height : appDetails.height

                ScrollBar.vertical: ScrollBar {
                    active: true
                }

                Parts.HorizontalListView {
                    id: sectionCaptures

                    visible: app['screenshot_links'].length > 0
                    height: 660
                    width: parent.width
                    clip: true
                    delegate: Image {
                        id: name
                        source: app['screenshot_links'][index]
                        width: 780
                        height: 580
                        fillMode: Image.PreserveAspectCrop

                        Label {
                            anchors.centerIn: parent
                            text: i18n("Loading...")
                            font.pixelSize: 18
                            opacity: 0.7
                            visible: parent.status == Image.Loading
                        }
                    }
                    model: app['screenshot_links']
                    snapMode: ListView.SnapToItem
                }

                PlasmaComponents.Label {

                    id: appDetails

                    anchors.top: sectionCaptures.visible ? sectionCaptures.bottom : parent.top
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.leftMargin: 6
                    anchors.rightMargin: 12

                    text: app['description']
                    wrapMode: Text.Wrap
                    font.pointSize: 11
                }
            }
        }
    }
}
