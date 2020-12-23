import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.kde.mauikit 1.3 as Maui
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

    //    flickable: _scrollablePage.flickable
    //    floatingHeader: true
    //    headerPositioning: ListView.PullBackHeader
    //    headerBackground.color: "transparent"

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
            _scrollablePage.flickable.contentY = _screenshotsSection.y;
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

    headBar.rightContent: [

    ToolButton
        {
            text: qsTr("Pling")
            icon.name: "pling"
            onClicked: Qt.openUrlExternally(appInfo.detailpage)
        },

        ToolButton
        {
            text: qsTr("Report")
            icon.name: "tools-report-bug"
            onClicked: Qt.openUrlExternally(appInfo.detailpage)

        }

    ]


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
            width: control.width
            spacing: Maui.Style.space.huge

            Item
            {
                id: _header
                Layout.preferredHeight: _bannerInfo.implicitHeight + Maui.Style.space.enormous
                Layout.fillWidth: true
                clip: true

                Image
                {
                    id: _bannerImage
                    anchors.centerIn: parent
                    height: parent.height * 2
                    width: parent.width * 2
                    source: imagesInfo[0].pic
                    sourceSize.height: 100
                    sourceSize.width: 100
                    fillMode: Image.PreserveAspectCrop
                    antialiasing: true
                    smooth: true
                    asynchronous: true
                }

                FastBlur
                {
                    id: fastBlur
                    anchors.fill: _banner
                    source: _banner
                    radius: 64
                    transparentBorder: false
                }

                Rectangle
                {
                    anchors.fill: parent
                    color: Kirigami.Theme.viewBackgroundColor
                    opacity: 0.9
                }

                Maui.FlexListItem
                {
                    id: _bannerInfo
                    width: parent.width
                    //                    anchors.fill: parent
                    anchors.centerIn: parent
                    wide: root.isWide
                    iconSizeHint: Maui.Style.iconSizes.huge
                    template.imageBorder: false
                    imageSource: _bannerImage.source
                    label1.text: appInfo.name
                    label1.elide: Text.ElideMiddle
                    label1.wrapMode: Text.NoWrap
                    label1.font.weight: Font.Bold
                    label1.font.bold: true
                    label1.font.pointSize: Maui.Style.fontSizes.enormous
                    //                    label2.text: appInfo.typename
                    template.leftLabels.spacing: Maui.Style.space.medium
                    rowSpacing: Maui.Style.space.big
                    template.spacing: Maui.Style.space.huge
                    //                    label3.text: appInfo.personid


                    template.leftLabels.data: Row
                    {
                        id: _actionButtons
                        //                        Layout.fillWidth: parent.wide
                        //                        Layout.margins: Maui.Style.space.big
                        //                        spacing: Maui.Style.space.medium
                        Layout.preferredHeight: implicitHeight
                    }

                    RowLayout
                    {
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignCenter
                        spacing: Maui.Style.space.big

                        Maui.GridItemTemplate
                        {
                            Layout.fillWidth: true
                            implicitWidth:  64
                            implicitHeight: 64
                            iconSource: "rating"
                            iconSizeHint: Maui.Style.iconSizes.medium
                            label1.text: appInfo.score
                            label1.font.bold: true
                            label1.font.weight: Font.Bold
                            label1.font.pointSize: Maui.Style.fontSizes.big
                        }

                        Maui.GridItemTemplate
                        {
                            Layout.fillWidth: true
                            implicitWidth:  64
                            implicitHeight: 64
                            iconSource: "download"
                            iconSizeHint: Maui.Style.iconSizes.medium
                            label1.text: appInfo.totaldownloads
                            label1.font.bold: true
                            label1.font.weight: Font.Bold
                            label1.font.pointSize: Maui.Style.fontSizes.big
                        }

                        Maui.GridItemTemplate
                        {
                            Layout.fillWidth: true

                            implicitWidth:  64
                            implicitHeight: 64

                            iconSource: "license"
                            iconSizeHint: Maui.Style.iconSizes.medium
                            label1.text: appInfo.license
                        }
                    }

                }

                Maui.Separator
                {
                    position: Qt.Horizontal
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                }
            }



            ListView
            {
                id: _screenshotsSection

                Layout.fillWidth: true
                Layout.preferredHeight: 200
                Layout.margins: Maui.Style.space.big
                clip: true
                model: control.imagesInfo
                spacing:  Maui.Style.space.big
                orientation: ListView.Horizontal
                //                snapMode: ListView.SnapOneItem

                delegate: Rectangle
                {
                    id: _delegate
                    color: Qt.tint(Kirigami.Theme.textColor, Qt.rgba(Kirigami.Theme.backgroundColor.r, Kirigami.Theme.backgroundColor.g, Kirigami.Theme.backgroundColor.b, 0.9))
                    height: ListView.view.height
                    width: Math.max(100, ListView.view.width * 0.3)

                    Image
                    {
                        width: parent.width
                        fillMode: Image.PreserveAspectCrop

                        anchors.margins: Maui.Style.space.big
                        source: modelData.pic

                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignHCenter
                    }

                    layer.enabled: true
                    layer.effect: OpacityMask
                    {
                        maskSource: Item
                        {
                            width: _delegate.width
                            height: _delegate.height

                            Rectangle
                            {
                                anchors.fill: parent
                                radius: Maui.Style.radiusV
                            }
                        }
                    }
                }
            }

            Maui.ListItemTemplate
            {
                id: _div1
                Layout.fillWidth: true

                label1.text: i18n("About the package")
                label2.text: appInfo.description
                label2.wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }


            Row
            {
                Layout.fillWidth: true
                Layout.preferredHeight: Maui.Style.toolBarHeight
                spacing: Maui.Style.space.big
                Repeater
                {
                    model: String(appInfo.tags).split(",")

                    Maui.Chip
                    {
//                        size: Maui.Style.iconSizes.medium
                        width: implicitWidth
                        label.text: modelData
                        iconSource: "tag"
//                        radius: Maui.Style.radiusV
                    }

                }
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
                            iconSource: Maui.FM.iconName(info.name)
                        }

                        onClicked:
                        {
                            control.packageClicked(index)
                        }

                    }
                }
            }
        }
    }
}
