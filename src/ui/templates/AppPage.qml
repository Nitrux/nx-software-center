import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.kde.mauikit 1.0 as Maui
import QtGraphicalEffects 1.0

Maui.Page
{
    id: control
    property var appInfo : ({})
    property alias buttonActions : _actionButtons.data
    signal exit()
    padding: 0
    onGoBackTriggered: control.exit()

    headBar.leftContent: [
        ToolButton
        {
            icon.name: "go-previous"
            onClicked: control.exit()
        },

        Kirigami.Separator
        {
            Layout.preferredHeight: Kirigami.Units.iconSizes.small
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
                    source: appInfo.bannerImage
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
                    radius: 50
                    transparentBorder: false
                    cached: true
                }

                Rectangle
                {
                    anchors.fill: parent
                    color: Kirigami.Theme.viewBackgroundColor
                    opacity: 0.7
                }

                ColumnLayout
                {
                    id: _bannerInfo
                    anchors.centerIn: parent
                    spacing: 0

                    Kirigami.Icon
                    {
                        Layout.preferredHeight: Kirigami.Units.iconSizes.large
                        Layout.preferredWidth: Kirigami.Units.iconSizes.large
                        Layout.alignment: Qt.AlignCenter
                        Layout.margins: Maui.Style.space.big
                        source: appInfo.iconName
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
                        text: appInfo.author
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
                        text: appInfo.size
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
                    Layout.preferredHeight: Kirigami.Units.iconSizes.smallMedium
                    Layout.preferredWidth: Kirigami.Units.iconSizes.smallMedium
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
                        text: appInfo.itemInfo
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
                    Layout.preferredHeight: Kirigami.Units.iconSizes.smallMedium
                    Layout.preferredWidth: Kirigami.Units.iconSizes.smallMedium
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

                    Label
                    {
                        Layout.fillWidth: true
                        text: qsTr("License") + " " + appInfo.license
                        elide: Text.ElideRight
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    }

                    Label
                    {
                        Layout.fillWidth: true
                        text: qsTr("Version") + " " + appInfo.version
                        elide: Text.ElideRight
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    }
                    Label
                    {
                        Layout.fillWidth: true
                        text: qsTr("Updated") + " " + appInfo.updated
                        elide: Text.ElideRight
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    }

                    Label
                    {
                        Layout.fillWidth: true
                        text: qsTr("Downloads today") + " " + appInfo.downloadsToday
                        elide: Text.ElideRight
                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
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
                    Layout.preferredHeight: Kirigami.Units.iconSizes.smallMedium
                    Layout.preferredWidth: Kirigami.Units.iconSizes.smallMedium
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
