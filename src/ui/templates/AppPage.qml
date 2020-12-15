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

    property bool shouldAnimateScroll: false
    readonly property int scrollAnimationDuration: 1000

    flickable: _scrollablePage.flickable
    floatingHeader: true
    headerPositioning: ListView.PullBackHeader
    headerBackground.color: "transparent"

    enum Sections {
        Description,
        Details,
        Packages,
        Screenshots,
        Changelog,
        Comments
    }

    signal exit()
    signal packageClicked(int index)

    function scrollTo(section) {
        shouldAnimateScroll = true;

        switch (section) {
        case AppPage.Sections.Description:
            _scrollablePage.flickable.contentY = _div1.y;
            break;
        case AppPage.Sections.Details:
            _scrollablePage.flickable.contentY = _div2.y;
            break;
        case AppPage.Sections.Packages:
            _scrollablePage.flickable.contentY = _div3.y;
            break;
        case AppPage.Sections.Screenshots:
            _scrollablePage.flickable.contentY = _div4.y;
            break;
        case AppPage.Sections.Changelog:
            _scrollablePage.flickable.contentY = _div5.y;
            break;
        case AppPage.Sections.Comments:
            _scrollablePage.flickable.contentY = _div6.y;
            break;
        }

        scrollAnimationResetTimer.start()
    }

    onGoBackTriggered: control.exit()

    Timer {
        id: scrollAnimationResetTimer
        interval: scrollAnimationDuration
        repeat: false
        onTriggered: {
            shouldAnimateScroll = false;
        }
    }

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
        Layout.fillWidth: true
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
            //            Action
            //            {
            //                text: qsTr("Favorite")
            //                icon.name: "love"
            //            },
            Action
            {
                text: qsTr("Pling")
                icon.name: "headphones"
                onTriggered: Qt.openUrlExternally(appInfo.detailpage)
            },
            Action
            {
                text: qsTr("Packages")
                icon.name: "media-playlist-append"
                onTriggered: {
                    scrollTo(AppPage.Sections.Packages);
                }
            },
            Action
            {
                text: qsTr("Screenshots")
                icon.name: "image-multiple"
                onTriggered: {
                    scrollTo(AppPage.Sections.Screenshots);
                }
            }
        ]
    }


    Kirigami.ScrollablePage
    {
        id: _scrollablePage
        anchors.fill: parent
        padding: 0
        leftPadding: padding
        rightPadding: padding
        topPadding: padding
        bottomPadding: padding
        flickable {
            Behavior on contentX {
                enabled: shouldAnimateScroll

                NumberAnimation {
                    duration: scrollAnimationDuration
                    easing.type: Easing.InOutQuad
                }
            }

            Behavior on contentY {
                enabled: shouldAnimateScroll

                NumberAnimation {
                    duration: scrollAnimationDuration
                    easing.type: Easing.InOutQuad
                }
            }
        }

        ColumnLayout
        {
            width: parent.width
            spacing: 0

            Item
            {
                id: _header
                Layout.preferredHeight: 230
                Layout.fillWidth: true

                Rectangle
                {
                    id: _banner
                    height: parent.height
                    width: parent.width
                    color: "#333"
                    opacity: 0.2

                    Image
                    {
                        id: _bannerImage
                        anchors.fill: parent
                        source: imagesInfo[0].pic
                        sourceSize.height: parent.height /50
                        sourceSize.width: parent.width /50
                        fillMode: Image.PreserveAspectCrop
                        antialiasing: true
                        smooth: true
                        asynchronous: true
                    }
                }

                FastBlur
                {
                    id: fastBlur
                    anchors.fill: _banner
                    y:1
                    source: _banner
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

                Maui.ListItemTemplate
                {
                    id: _bannerInfo
                    anchors.fill: parent
                    anchors.centerIn: parent
                    spacing: 0

                    iconSizeHint: Maui.Style.iconSizes.huge
                    imageBorder: false
                    imageSource: _bannerImage.source
                    label1.text: appInfo.name
                    label1.elide: Text.ElideMiddle
                    label1.wrapMode: Text.NoWrap
                    label1.font.weight: Font.Bold
                    label1.font.bold: true
                    label1.font.pointSize: Maui.Style.fontSizes.huge
                    label2.text: appInfo.category
                    label3.text: appInfo.personid


                    Row
                    {
                        id: _actionButtons
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

            ColumnLayout
            {
                id: _div1

                Layout.fillWidth: true
                Layout.margins: Maui.Style.space.big

                RowLayout
                {
                    Layout.fillWidth: true
                    Layout.preferredHeight: Maui.Style.toolBarHeight
                    Layout.margins: Maui.Style.space.medium
                    spacing:  Maui.Style.space.big

                    Kirigami.Icon
                    {
                        visible: isWide
                        Layout.preferredHeight: Maui.Style.iconSizes.medium
                        Layout.preferredWidth: Maui.Style.iconSizes.medium
                        Layout.alignment: Qt.AlignHCenter
                        source: "view-list-details"
                    }

                    Label
                    {
                        Layout.fillWidth: true
                        text: qsTr("Description")
                        font.weight: Font.Bold
                        font.bold: true
                    }
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


            Kirigami.Separator
            {
                Layout.fillWidth: true
            }

            ColumnLayout
            {
                id: _div2

                Layout.fillWidth: true
                Layout.preferredHeight: implicitHeight
                Layout.margins: Maui.Style.space.big

                RowLayout
                {
                    Layout.fillWidth: true
                    Layout.preferredHeight: Maui.Style.toolBarHeight
                    Layout.margins: Maui.Style.space.medium
                    spacing:  Maui.Style.space.big

                    Kirigami.Icon
                    {
                        visible: isWide
                        Layout.preferredHeight: Maui.Style.iconSizes.medium
                        Layout.preferredWidth: Maui.Style.iconSizes.medium
                        Layout.alignment: Qt.AlignHCenter
                        source: "media-playlist-append"
                    }

                    Label
                    {
                        Layout.fillWidth: true
                        text: qsTr("Details")
                        font.weight: Font.Bold
                        font.bold: true
                    }
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


            Kirigami.Separator
            {
                Layout.fillWidth: true
            }


            ColumnLayout
            {
                id: _div3
                Layout.fillWidth: true
                Layout.preferredHeight: implicitHeight
                Layout.margins: Maui.Style.space.big

                RowLayout
                {
                    Layout.fillWidth: true
                    Layout.preferredHeight: Maui.Style.toolBarHeight
                    Layout.margins: Maui.Style.space.medium
                    spacing:  Maui.Style.space.big

                    Kirigami.Icon
                    {
                        visible: isWide
                        Layout.preferredHeight: Maui.Style.iconSizes.medium
                        Layout.preferredWidth: Maui.Style.iconSizes.medium
                        Layout.alignment: Qt.AlignHCenter
                        source: "media-playlist-append"
                    }

                    Label
                    {
                        Layout.fillWidth: true
                        text: qsTr("Packages")
                        font.weight: Font.Bold
                        font.bold: true
                    }
                }

                Maui.GridView
                {
                    id: _packagesGrid
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: control.downloadsInfo
                    adaptContent: false
                    itemWidth: width * 0.5
                    itemHeight: 100

                    delegate: Maui.ItemDelegate
                    {
                        property var info : modelData
                        Kirigami.Theme.backgroundColor: "grey"
                        Kirigami.Theme.textColor: "white"
                        Kirigami.Theme.highlightColor: "#333"

                        width: _packagesGrid.cellWidth * 0.9
                        height: _packagesGrid.cellHeight * 0.9

                        Maui.ListItemTemplate
                        {
                            anchors.fill: parent
                            label1.text: info.name
                            label1.font.weight: Font.Bold
                            label1.font.bold: true
                            label2.text: info.tags
                            label3.text: info.packageArch
                            label4.text: info.size
                            iconSource: "appimage-store"
                        }

                        onClicked:
                        {
                            control.packageClicked(index)
                        }

                    }
                }
            }

            ColumnLayout
            {
                id: _div4
                Layout.fillWidth: true
                Layout.preferredHeight: implicitHeight
                Layout.margins: Maui.Style.space.big

                RowLayout
                {
                    Layout.fillWidth: true
                    Layout.preferredHeight: Maui.Style.toolBarHeight
                    Layout.margins: Maui.Style.space.medium
                    spacing:  Maui.Style.space.big

                    Kirigami.Icon
                    {
                        visible: isWide
                        Layout.preferredHeight: Maui.Style.iconSizes.medium
                        Layout.preferredWidth: Maui.Style.iconSizes.medium
                        Layout.alignment: Qt.AlignHCenter
                        source: "folder-pictures"
                    }

                    Label
                    {
                        Layout.fillWidth: true
                        text: qsTr("Screenshots")
                        font.weight: Font.Bold
                        font.bold: true
                    }
                }

                ListView
                {
                    Layout.fillWidth: true
                    Layout.preferredHeight: 500
                    clip: true
                    model: control.imagesInfo

                    orientation: ListView.Horizontal
                    snapMode: ListView.SnapOneItem

                    delegate:Item
                    {
                        height: _div4.height
                        width: _div4.width

                        Maui.ImageViewer
                        {
                            anchors.fill: parent
                            anchors.margins: Maui.Style.space.big
                            source: modelData.pic
                        }
                    }
                }
            }

            ColumnLayout
            {
                id: _div5

                Layout.fillWidth: true
                Layout.preferredHeight: implicitHeight
                Layout.margins: Maui.Style.space.big

                RowLayout
                {
                    Layout.fillWidth: true
                    Layout.preferredHeight: Maui.Style.toolBarHeight
                    Layout.margins: Maui.Style.space.medium
                    spacing:  Maui.Style.space.big

                    Kirigami.Icon
                    {
                        visible: isWide
                        Layout.preferredHeight: Maui.Style.iconSizes.medium
                        Layout.preferredWidth: Maui.Style.iconSizes.medium
                        Layout.alignment: Qt.AlignHCenter
                        source: "media-playlist-append"
                    }

                    Label
                    {
                        Layout.fillWidth: true
                        text: qsTr("Changelog")
                        font.weight: Font.Bold
                        font.bold: true
                    }
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


            ColumnLayout
            {
                id: _div6

                Layout.fillWidth: true
                Layout.preferredHeight: implicitHeight
                Layout.margins: Maui.Style.space.big

                RowLayout
                {
                    Layout.fillWidth: true
                    Layout.preferredHeight: Maui.Style.toolBarHeight
                    Layout.margins: Maui.Style.space.medium
                    spacing:  Maui.Style.space.big

                    Kirigami.Icon
                    {
                        visible: isWide
                        Layout.preferredHeight: Maui.Style.iconSizes.medium
                        Layout.preferredWidth: Maui.Style.iconSizes.medium
                        Layout.alignment: Qt.AlignHCenter
                        source: "media-playlist-append"
                    }

                    Label
                    {
                        Layout.fillWidth: true
                        text: qsTr("Comments")
                        font.weight: Font.Bold
                        font.bold: true
                    }
                }

                Column
                {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Repeater
                    {
                        model: control.appInfo.comments.split(",")
                        Label
                        {
                            Layout.fillWidth: true
                            font.weight: Font.Light
                            elide: Text.ElideRight
                            wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                            text: modelData
                        }

                    }
                }
            }

        }
    }
}
