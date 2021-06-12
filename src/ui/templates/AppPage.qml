import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3

import org.kde.kirigami 2.7 as Kirigami
import org.mauikit.controls 1.3 as Maui
import org.mauikit.filebrowsing 1.0 as FB
import org.mauikit.imagetools 1.0 as IT

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
            _scrollablePage.flickable.contentY = _div2.y;
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

    title: appInfo.name
    headBar.leftContent: ToolButton
    {
        icon.name: "go-previous"
        onClicked: control.exit()
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
            width: control.width
            spacing: Maui.Style.space.big

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
                    anchors.fill: _bannerImage
                    source: _bannerImage
                    radius: 64
                    transparentBorder: false
                }

                Rectangle
                {
                    anchors.fill: parent
                    color: control.Kirigami.Theme.backgroundColor
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
                    imageSource: _bannerImage.source
                    label1.text: appInfo.name
                    label1.elide: Text.ElideMiddle
                    label1.wrapMode: Text.WrapAnywhere
                    label1.font.weight: Font.Bold
                    label1.font.bold: true
                    label1.font.pointSize: Maui.Style.fontSizes.enormous
                    label2.text: String("<a href='%1'>%1</a>").arg(appInfo.personid)
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
                            label1.text: appInfo.license || i18n("Unkown")
                        }
                    }
                }

                Maui.Separator
                {
                    edge: Qt.BottomEdge
                    anchors.bottom: parent.bottom
                    anchors.left: parent.left
                    anchors.right: parent.right
                }
            }

            Maui.ListBrowser
            {
                id: _screenshotsSection
                verticalScrollBarPolicy: ScrollBar.AlwaysOff

                Kirigami.Theme.colorSet: Kirigami.Theme.Window
                Kirigami.Theme.inherit: false

                Layout.fillWidth: true
                Layout.preferredHeight: 500
                model: control.imagesInfo
                spacing: 0
                orientation: ListView.Horizontal

                flickable.highlightFollowsCurrentItem: true
                flickable.highlightMoveDuration: 0
                snapMode: ListView.SnapOneItem
                flickable.highlightRangeMode: ListView.StrictlyEnforceRange
                flickable.keyNavigationEnabled: true
                flickable.keyNavigationWraps : true

                delegate: MouseArea
                {
                    height: ListView.view.height
                    width: ListView.view.width

                    onDoubleClicked:
                    {
                        _imageViewerDialog.source = modelData.pic
                        _imageViewerDialog.open()
                    }

                    Image
                    {
                        anchors.fill: parent
                        fillMode: Image.PreserveAspectFit

                        source: modelData.pic

                        verticalAlignment: Qt.AlignVCenter
                        horizontalAlignment: Qt.AlignHCenter
                    }
                }
            }

            SectionTitle
            {
                label1.text: i18n("About")
                label2.text: i18n("Information about the application.")
            }

            Maui.ListItemTemplate
            {
                id: _div1
                Layout.fillWidth: true
                Layout.margins: Maui.Style.space.medium

                label1.text: appInfo.name
                label2.text: appInfo.description
                label2.wrapMode: Text.WrapAtWordBoundaryOrAnywhere
            }

            Maui.ListBrowser
            {
                Layout.fillWidth: true
                Layout.preferredHeight: Maui.Style.toolBarHeight* 1.5
                Layout.margins: Maui.Style.space.medium
                verticalScrollBarPolicy: ScrollBar.AlwaysOff

                spacing: Maui.Style.space.big

                orientation: ListView.Horizontal
                model: String(appInfo.tags).split(",")

                delegate: Maui.Chip
                {
                    showCloseButton: false
                    width: implicitWidth
                    label.text: modelData
                    iconSource: "tag"
                    //                        radius: Maui.Style.radiusV
                }
            }

            Maui.Separator
            {
                Layout.fillWidth: true
                edge: Qt.BottomEdge
            }

            SectionTitle
            {
                id: _div2
                label1.text: i18n("Packages")
                label2.text: i18n("Avaliable packages to download")
            }

            Maui.GridView
            {
                id: _packagesGrid
                Layout.fillWidth: true
                Layout.fillHeight: true
                Layout.margins: Maui.Style.space.medium
                model: control.downloadsInfo
                adaptContent: true
                itemSize: 360
                itemHeight: 100

                delegate: FloatingCardDelegate
                {
                    id: _delegate
                    property var info : modelData

                    width: GridView.view.cellWidth
                    height: GridView.view.cellHeight

                    label1.text: info.name
                    label1.font.pointSize: Maui.Style.fontSizes.huge
                    label1.font.weight: Font.Bold
                    label1.font.bold: true
                    label1.wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                    //                        label2.text: info.tags
                    label3.text: info.packageArch
                    label2.text: info.size
                    iconSource: FB.FM.iconName(info.name)
                    iconSizeHint: Maui.Style.iconSizes.large

                    onDoubleClicked:
                    {
                        animate( _delegate.mapToItem(control, 0, 0), FB.FM.iconName(info.name))
                        control.packageClicked(index)
                    }
                }
            }
        }
    }

    Kirigami.Icon
    {
        id: _aniImg
        visible: _aniX.running
        parent: ApplicationWindow.overlay
        source: imagesInfo[0].pic
        height: 60
        width: 60

        property point endPos

        NumberAnimation on height
        {
            running: _aniY.running
            from: 120
            to: Maui.Style.iconSizes.medium
            duration: _aniY.duration
        }

        NumberAnimation on width
        {
            running: _aniY.running
            from: 120
            to: Maui.Style.iconSizes.medium
            duration: _aniY.duration
        }

        NumberAnimation on x
        {
            id: _aniX
            running: false
            from: _aniImg.x; to: _aniImg.endPos.x + _swipeView.actionGroup.width - (_aniImg.width)
            duration: Kirigami.Units.longDuration * 2
            loops: 1
            easing.type: Easing.OutQuad
        }

        NumberAnimation on y
        {
            id: _aniY
            running: false
            easing.type: Easing.OutQuad
            from: _aniImg.y; to: _aniImg.endPos.y + _aniImg.height
            duration:  Kirigami.Units.longDuration * 2.5
            loops: 1
        }

        Connections
        {
            target: _aniY
            function onFinished()
            {
                goToProgressView()
            }
        }
    }

    function goToProgressView()
    {
        _swipeView.currentIndex = root.views.progress
    }

    function animate(pos, icon)
    {
        _aniImg.source = icon

        _aniImg.endPos = _swipeView.actionGroup.mapToItem(control, 0, 0)

        _aniImg.x = pos.x
        _aniImg.y = pos.y

        _aniX.start()
        _aniY.start()
    }


    Popup
    {
        id: _imageViewerDialog
        parent: root
        background: null
        modal: true

        property alias source :_imageViewer.source
        height: root.height
        width: root.width

        IT.ImageViewer
        {
            id: _imageViewer

            anchors.fill: parent
            anchors.margins: Maui.Style.space.huge
        }
    }
}
