import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3

import org.kde.plasma.core 2.0 as PlasmaCore

import org.nxos.softwarecenter 1.0

ColumnLayout {
    Item {
        Layout.preferredHeight: 300
        Layout.fillWidth: true
        Layout.margins: 12

        Image {
            id: background
            anchors.fill: parent
            source: ApplicationViewController.backgroundImage
            fillMode: Image.PreserveAspectCrop

            opacity: 0.3

            visible: background.source != "" && background.status == Image.Ready

        }

        Image {
            id: fallbackBackground
            anchors.fill: parent

            source: "qrc:/images/backgrounds/internet.png"
            fillMode: Image.PreserveAspectCrop
            opacity: 0.3

            visible: !background.visible
        }

        ColumnLayout {
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.margins: 16

            spacing: 0

            Image {
                id: iconImage
                Layout.preferredHeight: 100
                Layout.preferredWidth: 100
                Layout.alignment: Qt.AlignCenter
                Layout.margins: 10
                source: ApplicationViewController.appIcon;

                visible: source != "" && status == Image.Ready
            }

            PlasmaCore.IconItem {
                id: placeHolderIconImage
                Layout.preferredHeight: 100
                Layout.preferredWidth: 100
                Layout.alignment: Qt.AlignCenter
                Layout.margins: 10

                source: "package-x-generic"
                visible: !iconImage.visible
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                text: ApplicationViewController.appName;
                font.pointSize: 18
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                text: i18n("by ") + ApplicationViewController.appAuthor;
                font.pointSize: 16
                visible: ApplicationViewController.appAuthor
            }

            Text {
                Layout.alignment: Qt.AlignHCenter
                Layout.topMargin: 12
                Layout.maximumWidth: 280

                text: i18n("The application is being changed, please wait");
                font.pointSize: 10
                wrapMode: Text.WordWrap

                visible: ApplicationViewController.hasPendingTasks
            }

            RowLayout {
                visible: !ApplicationViewController.hasPendingTasks;
                Layout.topMargin: 16
                Layout.alignment: Qt.AlignHCenter
                spacing: 16

                Button {
                    id: getButton
                    text: i18n("Get")
                    visible: !ApplicationViewController.isAppInstalled;

                    onReleased: InstallController.install(ApplicationViewController.appId)
                }

                Button {
                    id: runButton
                    text: i18n("Run")

                    visible: ApplicationViewController.isAppInstalled

                    onReleased: RunController.run(ApplicationViewController.appId)
                }

                Button {
                    id: removeButton
                    text: i18n("Remove")

                    visible: ApplicationViewController.isAppInstalled

                    onReleased: UninstallController.uninstall(ApplicationViewController.appId)
                }
            }

        }
    }

    RowLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true
        Layout.margins: 12

        ColumnLayout {
            id: informationColumn
            Layout.alignment: Qt.AlignTop | Qt.AlignLeft
            spacing: 12

            Text {
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                text: i18n("Information")
                font.pointSize: 16
            }

            Text {
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                text: i18n("- Website: %1".arg(ApplicationViewController.appWebsite))
                font.pointSize: 8

                visible: ApplicationViewController.appWebsite
            }

            Text {
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                text: i18n("- Version: %1".arg(ApplicationViewController.appVersion))
                font.pointSize: 8

                visible: ApplicationViewController.appVersion
            }

            Text {
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                text: i18n("- Size: %1".arg(ApplicationViewController.appDownloadSize))
                font.pointSize: 8

                visible: ApplicationViewController.appDownloadSize
            }

            Text {
                Layout.alignment: Qt.AlignTop | Qt.AlignLeft
                Layout.fillHeight: true;
                Layout.fillWidth: true;

                wrapMode: Text.WordWrap

                text: '<font size="2">' + ApplicationViewController.appDescription + '</font>'
                textFormat: Text.RichText
            }
        }
    }

}
