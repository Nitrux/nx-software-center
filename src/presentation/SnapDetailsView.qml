import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.nx.softwarecenter 1.0

import "parts" as Parts

Item {
    id: root
    width: 800
    height: 600
    property string package_name: "minecraft-nsg"
    property var details

    Component.onCompleted: {
        var request = SnapStore.getSnapDetails(package_name)
        request.complete.connect(function () {

            details = request.snapDetails()

            //            for (var k in details) {
            //                print(k, details[k])
            //            }
            contentLoader.sourceComponent = detailsView
        })

        request.runAsync()
    }

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

                            visible: details.icon_url === undefined
                            source: "package-available"
                        }

                        Image {
                            anchors.fill: parent
                            anchors.margins: 12
                            visible: details.icon_url !== undefined
                            source: visible ? details.icon_url : ""
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

                        text: root.details.title
                        wrapMode: Text.WordWrap
                        font.pointSize: 18
                    }

                    GridLayout {
                        Layout.alignment: Qt.AlignTop
                        Layout.fillWidth: true

                        Repeater {
                            model: details.keywords
                            delegate: PlasmaComponents.Label {
                                text: details.keywords[index]
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
                            text: details.license
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
                        text: details.publisher
                        wrapMode: Text.WordWrap
                    }

                    Parts.RatingStars {
                        id: snapStars
                        Layout.topMargin: 18
                        rating: details.ratings_average
                    }
                }
            }

            Flickable {
                Layout.columnSpan: 3
                Layout.margins: 12
                Layout.fillWidth: true
                Layout.preferredHeight: 500

                clip: true
                visible: root.details.screenshot_urls.length > 0

                contentWidth: screenshotsLayout.width
                contentHeight: screenshotsLayout.height

                RowLayout {
                    id: screenshotsLayout
                    Repeater {
                        model: root.details.screenshot_urls
                        delegate: Image {
                            source: root.details.screenshot_urls[index]
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
                text: root.details.description

                wrapMode: Text.Wrap
            }
        }
    }

    Component {
        id: statusView
        StatusArea {
        }
    }
}
