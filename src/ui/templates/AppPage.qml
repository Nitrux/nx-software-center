import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.kde.mauikit 1.0 as Maui
import QtGraphicalEffects 1.0

import NXModels 1.0 as NX

Maui.Page
{
    id: control
    property alias appInfo : _appHandler.info
    property alias imagesInfo : _appHandler.images
    property alias downloadsInfo : _appHandler.downloads
    property alias urlsInfo : _appHandler.urls

    property alias buttonActions : _actionButtons.data
    property alias data : _appHandler.data
    property alias app : _appHandler

    signal exit()
    padding: 0
    onGoBackTriggered: control.exit()

    NX.App
    {
        id: _appHandler
    }

    headBar.leftContent: [
        ToolButton
        {
            icon.name: "go-previous"
            onClicked: control.exit()
        },

        Kirigami.Separator
        {
            Layout.preferredHeight: Maui.Style.iconSizes.small
            width:  2
        }
    ]

    headBar.rightContent: Kirigami.ActionToolBar
    {
        display: isWide ? ToolButton.TextBesideIcon : ToolButton.IconOnly
        hiddenActions: [
            Action
            {
                text: qsTr("Report spam")
            },
            Action
            {
                text: qsTr("Report missused")
            }
        ]

        actions: [
            Action
            {
                text: qsTr("Favorite")
                icon.name: "love"
            },

            Action
            {
                text: qsTr("Pling")
                icon.name: "headphones"
                onTriggered: Qt.openUrlExternally(appInfo.detailpage)
            },

            Action
            {
                text: qsTr("Screenshots")
                icon.name: "image-multiple"
            }

        ]
    }


    Kirigami.ScrollablePage
    {
        anchors.fill: parent
        padding: 0
        leftPadding: padding
        rightPadding: padding
        topPadding: padding
        bottomPadding: padding
        ColumnLayout
        {
            spacing: 0
            Item
            {
                id: _header
                Layout.preferredHeight: 230
                Layout.margins: 0
                Layout.fillWidth: true

                Image
                {
                    id: _bannerImage
                    anchors.fill: parent
                    source: imagesInfo[0].pic
                    sourceSize.height: parent.height
                    sourceSize.width: parent.width
                    fillMode: Image.PreserveAspectCrop
                    antialiasing: true
                    smooth: true
                    asynchronous: true
                }

                FastBlur
                {
                    id: fastBlur
                    anchors.fill: parent
                    y:1
                    source: _bannerImage
                    radius: 120
                    transparentBorder: false
                    cached: true
                }

                Rectangle
                {
                    anchors.fill: parent
                    color: Kirigami.Theme.viewBackgroundColor
                    opacity: 0.8
                }

                ColumnLayout
                {
                    id: _bannerInfo
                    anchors.centerIn: parent
                    spacing: 0

                    Image
                    {
                        Layout.preferredHeight: Maui.Style.iconSizes.huge
                        Layout.preferredWidth: Maui.Style.iconSizes.huge
                        Layout.alignment: Qt.AlignCenter
                        Layout.margins: Maui.Style.space.big
                        source: _bannerImage.source

                        sourceSize.height: height
                        sourceSize.width: width
                        fillMode: Image.PreserveAspectFit
                        antialiasing: true
                        smooth: true
                        asynchronous: true
                    }

                    Label
                    {
                        Layout.fillWidth: true
                        text: appInfo.name
                        width: parent.width
                        Layout.alignment: Qt.AlignCenter
                        horizontalAlignment: Qt.AlignHCenter
                        elide: Text.ElideMiddle
                        wrapMode: Text.NoWrap
                        font.weight: Font.Bold
                        font.bold: true
                        font.pointSize: Maui.Style.fontSizes.big
                    }

                    Label
                    {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignCenter
                        horizontalAlignment: Qt.AlignHCenter
                        text: appInfo.personid
                        elide: Text.ElideMiddle
                        wrapMode: Text.NoWrap
                        font.weight: Font.Light
                        font.pointSize: Maui.Style.fontSizes.medium
                    }

                    Label
                    {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignCenter
                        horizontalAlignment: Qt.AlignHCenter
                        text: appInfo.version
                        elide: Text.ElideMiddle
                        wrapMode: Text.NoWrap
                        font.weight: Font.Light
                        font.pointSize: Maui.Style.fontSizes.medium
                    }

                    Row
                    {
                        id: _actionButtons
                        Layout.fillWidth: true
                        Layout.margins: Maui.Style.space.big
                        spacing: Maui.Style.space.medium
                        Layout.preferredHeight: implicitHeight
                    }
                }
            }

            Kirigami.Separator
            {
                Layout.fillWidth: true
            }

            RowLayout
            {
                id: _div1

                Layout.fillWidth: true
                Layout.preferredHeight: implicitHeight
                Layout.margins: Maui.Style.space.big

                Kirigami.Icon
                {
                    visible: isWide
                    Layout.preferredHeight: Maui.Style.iconSizes.medium
                    Layout.preferredWidth: Maui.Style.iconSizes.medium
                    Layout.margins: Maui.Style.space.big
                    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                    source: "view-list-details"
                }

                ColumnLayout
                {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Label
                    {
                        Layout.fillWidth: true
                        text: qsTr("Description")
                        font.weight: Font.Bold
                        font.bold: true
                    }

                    Label
                    {
                        Layout.fillWidth: true
                        font.weight: Font.Light
                        elide: Text.ElideRight
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        text: appInfo.description
                    }
                }
            }

            Kirigami.Separator
            {
                Layout.fillWidth: true
            }

            RowLayout
            {
                id: _div2

                Layout.fillWidth: true
                Layout.preferredHeight: implicitHeight
                Layout.margins: Maui.Style.space.big

                Kirigami.Icon
                {
                    visible: isWide
                    Layout.preferredHeight: Maui.Style.iconSizes.medium
                    Layout.preferredWidth: Maui.Style.iconSizes.medium
                    Layout.margins: Maui.Style.space.big
                    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                    source: "media-playlist-append"
                }

                ColumnLayout
                {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Label
                    {
                        Layout.fillWidth: true
                        text: qsTr("Details")
                        font.weight: Font.Bold
                        font.bold: true
                    }

                    Column
                    {
                        Label
                        {
                            Layout.fillWidth: true
                            text: qsTr("License")
                            elide: Text.ElideRight
                            wrapMode: Text.NoWrap
                            font.weight: Font.Light
                        }

                        Label
                        {
                            Layout.fillWidth: true
                            text: appInfo.license || "-"
                            elide: Text.ElideRight
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        }
                    }

                    Column
                    {
                        Label
                        {
                            Layout.fillWidth: true
                            text: qsTr("Version")
                            elide: Text.ElideRight
                            wrapMode: Text.NoWrap
                            font.weight: Font.Light
                        }

                        Label
                        {
                            Layout.fillWidth: true
                            text: appInfo.version || "-"
                            elide: Text.ElideRight
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        }
                    }

                    Column
                    {
                        Label
                        {
                            Layout.fillWidth: true
                            text: qsTr("Updated")
                            elide: Text.ElideRight
                            wrapMode: Text.NoWrap
                            font.weight: Font.Light
                        }

                        Label
                        {
                            Layout.fillWidth: true
                            text: appInfo.changed || "-"
                            elide: Text.ElideRight
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        }
                    }

                    Column
                    {
                        Label
                        {
                            Layout.fillWidth: true
                            text: qsTr("Created")
                            elide: Text.ElideRight
                            wrapMode: Text.NoWrap
                            font.weight: Font.Light
                        }

                        Label
                        {
                            Layout.fillWidth: true
                            text: appInfo.created || "-"
                            elide: Text.ElideRight
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        }
                    }

                    Column
                    {
                        Label
                        {
                            Layout.fillWidth: true
                            text: qsTr("Author")
                            elide: Text.ElideRight
                            wrapMode: Text.NoWrap
                            font.weight: Font.Light
                        }

                        Label
                        {
                            Layout.fillWidth: true
                            text: appInfo.personid || "-"
                            elide: Text.ElideRight
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        }
                    }

                    Column
                    {
                        Label
                        {
                            Layout.fillWidth: true
                            text: qsTr("Language")
                            elide: Text.ElideRight
                            wrapMode: Text.NoWrap
                            font.weight: Font.Light
                        }

                        Label
                        {
                            Layout.fillWidth: true
                            text: appInfo.language || "-"
                            elide: Text.ElideRight
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        }
                    }

                    Column
                    {
                        Label
                        {
                            Layout.fillWidth: true
                            text: qsTr("Tags")
                            elide: Text.ElideRight
                            wrapMode: Text.NoWrap
                            font.weight: Font.Light
                        }

                        Label
                        {
                            Layout.fillWidth: true
                            text: appInfo.tags || "-"
                            elide: Text.ElideRight
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        }
                    }

                    Column
                    {
                        Label
                        {
                            Layout.fillWidth: true
                            text: qsTr("Score")
                            elide: Text.ElideRight
                            wrapMode: Text.NoWrap
                            font.weight: Font.Light
                        }

                        Label
                        {
                            Layout.fillWidth: true
                            text: appInfo.score || "-"
                            elide: Text.ElideRight
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        }
                    }

                    Column
                    {
                        Label
                        {
                            Layout.fillWidth: true
                            text: qsTr("Downloads")
                            elide: Text.ElideRight
                            wrapMode: Text.NoWrap
                            font.weight: Font.Light
                        }

                        Label
                        {
                            Layout.fillWidth: true
                            text: appInfo.totaldownloads || "-"
                            elide: Text.ElideRight
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        }
                    }
                }
            }

            Kirigami.Separator
            {
                Layout.fillWidth: true
            }

            RowLayout
            {
                id: _div3

                Layout.fillWidth: true
                Layout.preferredHeight: implicitHeight
                Layout.margins: Maui.Style.space.big

                Kirigami.Icon
                {
                    visible: isWide
                    Layout.preferredHeight: Maui.Style.iconSizes.medium
                    Layout.preferredWidth: Maui.Style.iconSizes.medium
                    Layout.margins: Maui.Style.space.big
                    Layout.alignment: Qt.AlignTop | Qt.AlignHCenter
                    source: "view-media-recent"
                }

                ColumnLayout
                {
                    Layout.fillHeight: true
                    Layout.fillWidth: true

                    Label
                    {
                        Layout.fillWidth: true
                        text: qsTr("Changelog")
                        font.weight: Font.Bold
                        font.bold: true
                    }

                    Label
                    {
                        Layout.fillWidth: true
                        font.weight: Font.Light
                        elide: Text.ElideRight
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        text: appInfo.changelogVersion
                    }

                    Label
                    {
                        Layout.fillWidth: true
                        font.weight: Font.Light
                        elide: Text.ElideRight
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                        text: appInfo.changelogInfo
                    }
                }
            }
        }
    }
}
