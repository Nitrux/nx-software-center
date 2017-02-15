import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.nx.softwarecenter 1.0

ApplicationWindow {
    id: root
    width: 800
    height: 600
    property string package_name: "minecraft-nsg"
    property var details

    visible: true

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
        GridLayout {
            columns: 3
            Layout.preferredWidth: 600
            Item {
                Layout.rowSpan: 5
                Layout.preferredHeight: 222
                Layout.preferredWidth: 222

                PlasmaCore.IconItem {
                    id: snapIcon

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

            PlasmaComponents.Label {
                id: snapTitle
                Layout.topMargin: 16
                Layout.fillWidth: true
                text: root.details.title
                font.pointSize: 18
            }

            GridLayout {
                PlasmaComponents.Label {
                    text: i18n("Developed by")
                }

                PlasmaComponents.Label {
                    text: details.publisher
                }
            }

            GridLayout {
                columns: 3
                Layout.rowSpan: 3
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignTop

                Repeater {
                    model: details.keywords
                    delegate: PlasmaComponents.Label {
                        text: details.keywords[index]
                    }
                }
            }

            RowLayout {
                id: snapStars
                spacing: 0

                Layout.rowSpan: 3
                Layout.alignment: Qt.AlignTop

                Repeater {
                    model: details.ratings_average
                    delegate: PlasmaCore.IconItem {
                        source: "starred-symbolic"
                    }
                }

                Repeater {
                    model: 5 - details.ratings_average
                    delegate: PlasmaCore.IconItem {
                        Layout.alignment: Qt.AlignTop
                        source: "non-starred-symbolic"
                    }
                }
            }

            RowLayout {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignBottom
                PlasmaCore.IconItem {
                    Layout.preferredHeight: 24
                    Layout.preferredWidth: 24
                    source: "license"
                }

                PlasmaComponents.Label {
                    text: details.license
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
