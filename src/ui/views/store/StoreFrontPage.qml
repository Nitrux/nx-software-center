import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.14 as Kirigami
import org.kde.mauikit 1.2 as Maui
import QtGraphicalEffects 1.0

import "../../templates"
import NXModels 1.0 as NX

Maui.Page
{
    id: control

    signal itemClicked(var app)

    //    headerBackground.color: "transparent"
    headBar.middleContent: Maui.TextField
    {
        Layout.fillWidth: true
        Layout.maximumWidth: 500

        //        implicitHeight: Maui.Style.toolBarHeight
        placeholderText: i18n ("Search your app")
    }

    Maui.GridView
    {
        id: _featureGridView
        anchors.fill: parent
        //        flickable.topMargin: Maui.Style.space.big
        itemSize: Math.floor(Math.min(320, Math.max(100, control.width * 0.3)))
        itemHeight: 200

        model: Maui.BaseModel
        {
            list: NX.Store
            {
                id: _featureList
                //                                category: _categoriesSidebar.list.featureCategory()
                pageSize: 10
                sort: NX.Store.MOST_DOWNLOADED
            }
        }

        delegate: FeatureGridCard
        {
            images: _app.images

            width: _featureGridView.cellWidth
            height: _featureGridView.cellHeight
//            iconSource: model.smallpic
//            iconSizeHint: Maui.Style.iconSizes.medium
            label1.text: model.name
            label2.text: model.personid
            label3.text: model.totaldownloads
            label4.text: i18n("Downloads")

            NX.App
            {
                id: _app
                data: _featureList.application(model.id)
            }

            onClicked:
            {
                _featureGridView.currentIndex = index
                _featureList.setApp(model.id)
                control.itemClicked(_featureList.app)
            }
        }

        flickable.header: ColumnLayout
        {
            width: GridView.view.width
            spacing: Maui.Style.space.big

            Item
            {
                height: 250
                Layout.fillWidth: true
                Layout.margins: Maui.Style.space.big
//                Maui.Separator
//                {
//                    z: 99999
//                    anchors.bottom: parent.bottom
//                    anchors.left: parent.left
//                    anchors.right: parent.right
//                    position: Qt.Horizontal
//                }

                Rectangle
                {
                    id: _featuredSection
                    height: 250
                    width: parent.width
                    anchors.centerIn: parent
                    Kirigami.Theme.colorSet: Kirigami.Theme.Button
                    Kirigami.Theme.inherit: false
                    color: Kirigami.Theme.backgroundColor
                    //                    border.color: Qt.tint(Kirigami.Theme.textColor, Qt.rgba(Kirigami.Theme.backgroundColor.r, Kirigami.Theme.backgroundColor.g, Kirigami.Theme.backgroundColor.b, 0.7))
                    //                    radius: Maui.Style.radiusV * 2

                    Rectangle
                    {
                        id: _banner
                        anchors.fill: parent

                        Image
                        {
                            id: _bannerImage
                            anchors.fill : parent

                            sourceSize.height: height /50
                            sourceSize.width: width /50
                            source: _featuredListview.currentItem.imageSource
                            fillMode: Image.PreserveAspectCrop
                            antialiasing: false
                            smooth: false
                            asynchronous: true

                        }
                    }

                    FastBlur
                    {
                        id: fastBlur
                        anchors.fill: _banner
                        source: _banner
                        radius: 90
                        transparentBorder: false
                        cached: true

                        layer.enabled: true
                        layer.effect: OpacityMask
                        {
                            maskSource: Item
                            {
                                width: fastBlur.width
                                height: fastBlur.height

                                Rectangle
                                {
                                    anchors.centerIn: parent
                                    width: fastBlur.width
                                    height: fastBlur.height
                                    radius: _featuredSection.radius
                                }
                            }
                        }
                    }

                    ListView
                    {
                        id: _featuredListview
                        anchors.fill: parent
                        orientation: ListView.Horizontal
                        snapMode: ListView.SnapOneItem
                        clip: true
                        highlightMoveDuration: 1000
                        highlightMoveVelocity: -1

                        onMovementEnded: currentIndex = indexAt(contentX, contentY)

                        model: Maui.BaseModel
                        {
                            list: NX.Store
                            {
                                id: _featureListBanner
                                //                                category: _categoriesSidebar.list.featureCategory()
                                pageSize: 2
                                sort: NX.Store.HIGHEST_RATED
                            }
                        }

                        MouseArea
                        {
                            id: _featureMouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            propagateComposedEvents: true
                        }

                        Timer
                        {
                            id: _featuredListviewTimer
                            interval: 7000
                            repeat: true
                            running: !_featureMouseArea.containsPress || !_featureMouseArea.containsMouse
                            onTriggered: _featuredListview.cycleSlideForward()
                        }

                        Row
                        {
                            spacing: Maui.Style.space.medium
                            anchors.horizontalCenter: parent.horizontalCenter
                            z: 999
                            anchors.bottom: parent.bottom
                            anchors.margins: Maui.Style.space.big

                            Repeater
                            {
                                model: _featureListBanner.count

                                Rectangle
                                {
                                    width: Maui.Style.iconSizes.tiny
                                    height: width
                                    radius: width

                                    color: "grey"
                                    opacity: index === _featuredListview.currentIndex ? 1 : 0.5
                                }
                            }
                        }

                        delegate: Maui.ItemDelegate
                        {
                            property url imageSource: model.preview
                            height: _featuredListview.height
                            width: _featuredListview.width

                            onClicked:
                            {
                                control.push(_appPageComponent)
                                _storeList.setApp(_featuredListview.model.get(_featuredListview.currentIndex).id)
                                control.currentItem.data = _storeList.app
                            }

                            Maui.ListItemTemplate
                            {
                                anchors.fill: parent

                                anchors.margins: Maui.Style.space.huge

                                label1.font.pointSize: Maui.Style.fontSizes.enormous * 2
                                label1.font.bold: true
                                label1.font.weight: Font.Bold
                                label1.text: model.name
                                label2.text: model.description
                                label2.wrapMode: Text.WordWrap

                                imageSource: model.preview
                                imageSizeHint:  Maui.Style.iconSizes.enormous

                                imageBorder: false

                                fillMode: Image.PreserveAspectFit
                            }
                        }

                        function cycleSlideForward() {
                            _featuredListviewTimer.restart();

                            if (_featuredListview.currentIndex === _featuredListview.count - 1) {
                                _featuredListview.currentIndex = 0;
                            } else {
                                _featuredListview.incrementCurrentIndex();
                            }
                        }

                        function cycleSlideBackward() {
                            _featuredListviewTimer.restart();

                            if (_featuredListview.currentIndex === 0) {
                                _featuredListview.currentIndex = _featuredListview.count - 1;
                            } else {
                                _featuredListview.decrementCurrentIndex();
                            }
                        }
                    }


                    layer.enabled: true
                    layer.effect: OpacityMask
                    {
                        maskSource: Item
                        {
                            width: _featuredSection.width
                            height: _featuredSection.height

                            Rectangle
                            {
                                anchors.centerIn: parent
                                width: _featuredSection.width
                                height: _featuredSection.height
                                radius: Maui.Style.radiusV * 2
                            }
                        }
                    }

                }
            }


//            Maui.ListItemTemplate
//            {
//                Layout.fillWidth: true
//                implicitHeight: leftLabels.implicitHeight
//                label1.text: i18n("Categories")
//                label1.font.pointSize: Maui.Style.fontSizes.enormous
//                label1.font.bold: true
//                label1.font.weight: Font.Bold
//                label2.text: i18n("Filter by category to find your match.")
//            }

            GridLayout
            {
                id: _categoriesListView
                //                Layout.fillWidth: true
                Layout.alignment: Qt.AlignHCenter
                Layout.margins: Maui.Style.space.big
//                implicitHeight: 140
                columns: 5

                //                orientation: Qt.Horizontal
                //                horizontalScrollBarPolicy: ScrollBar.AlwaysOff
                //                verticalScrollBarPolicy:  ScrollBar.AlwaysOff

                Repeater
                {
                    model: Maui.BaseModel
                    {
                        id: _categoriesModel
                        list: NX.Categories
                        {
                            id: _categoriesList
                        }
                    }

                    delegate: GridItemCard
                    {
                        width: 100
                        height: 120
                        iconSource: model.icon
                        iconSizeHint: Maui.Style.iconSizes.big
                        label1.text: model.title
                    }
                }
            }

            Maui.Separator
            {
                Layout.fillWidth: true

                position: Qt.Horizontal
            }

            Maui.Separator
            {
                Layout.fillWidth: true
                position: Qt.Horizontal
            }

            Maui.ListItemTemplate
            {
                Layout.fillWidth: true
                Layout.margins: Maui.Style.space.medium
                leftLabels.spacing: Maui.Style.space.small
                implicitHeight: leftLabels.implicitHeight
                label1.text: i18n("Newest")
                label1.font.pointSize: Maui.Style.fontSizes.enormous * 1.5
                label1.font.bold: true
                label1.font.weight: Font.Bold
                label2.font.pointSize: Maui.Style.fontSizes.huge
                label2.text: i18n("Most newest additions to our collection.")
            }

            Maui.ListBrowser
            {
                Layout.fillWidth: true
                implicitHeight: 120
                orientation: Qt.Horizontal
                spacing: Maui.Style.space.big
                horizontalScrollBarPolicy: ScrollBar.AlwaysOff

                verticalScrollBarPolicy: ScrollBar.AlwaysOff

                model: Maui.BaseModel
                {
                    list: NX.Store
                    {
                        id: _newestListModel
                        pageSize: 4
                        sort: NX.Store.NEWEST
                    }
                }

                delegate: Maui.ItemDelegate
                {
                    width: 220
                    height: 80

                    background: Rectangle
                    {
                        color: Qt.tint(control.Kirigami.Theme.textColor, Qt.rgba(control.Kirigami.Theme.backgroundColor.r, control.Kirigami.Theme.backgroundColor.g, control.Kirigami.Theme.backgroundColor.b, 0.9))
                        radius: Maui.Style.radiusV
                    }

                    Maui.ListItemTemplate
                    {
                        anchors.fill: parent

                        label1.text: model.name
                        label2.text: model.personid
                        label1.font.bold: true
                        label1.font.weight: Font.Bold
                        label1.font.pointSize: Maui.Style.fontSizes.enormous
                        label2.font.pointSize: Maui.Style.fontSizes.big
                        label3.font.pointSize: Maui.Style.fontSizes.big
                        label4.font.pointSize: Maui.Style.fontSizes.small
                        label3.font.bold: true
                        label3.font.weight: Font.Bold
                        imageSource: model.smallpic
                        imageBorder: false
                        imageSizeHint: Maui.Style.iconSizes.huge

                    }
                }
            }

            Maui.Separator
            {
                Layout.fillWidth: true
                position: Qt.Horizontal
            }


            Maui.ListItemTemplate
            {
                Layout.fillWidth: true
                Layout.margins: Maui.Style.space.medium
                leftLabels.spacing: Maui.Style.space.small
                implicitHeight: leftLabels.implicitHeight
                label1.text: i18n("Most Popular")
                label1.font.pointSize: Maui.Style.fontSizes.enormous * 1.5
                label1.font.bold: true
                label1.font.weight: Font.Bold
                label2.font.pointSize: Maui.Style.fontSizes.huge
                label2.text: i18n("Most downloaded packages.")
            }

            Item{Layout.fillWidth: true}
        }

    }




    function setCurrentCategory(index)
    {
        _categoriesList.setCurrentCategory(control.model.get(index).id)
    }
}
