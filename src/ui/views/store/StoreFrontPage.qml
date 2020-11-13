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
    signal categoryClicked(var category)

    Kirigami.Theme.colorSet: Kirigami.Theme.View
    Kirigami.Theme.inherit: false

    NX.Categories
    {
        id: _categoriesList
    }

    headBar.middleContent: Maui.TextField
    {
        Layout.fillWidth: true
        Layout.maximumWidth: 500
        placeholderText: i18n ("Search your app package...")
    }

    Maui.GridView
    {
        id: _featureGridView
        anchors.fill: parent
        itemSize: Math.floor(Math.min(320, Math.max(200, control.width * 0.3)))
        itemHeight: 200

        model: Maui.BaseModel
        {
            list: NX.Store
            {
                id: _featureList
                category: _categoriesList.baseCategory()
                pageSize: 14
                sort: NX.Store.MOST_DOWNLOADED
            }
        }

        delegate: FeatureGridCard
        {
            images: _app.images

            width: _featureGridView.cellWidth
            height: _featureGridView.cellHeight
            label1.text: model.name
            label2.text: model.typename
            label3.text: model.totaldownloads
            label4.text: model.personid

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
            width: _featureGridView.flickable.width
            spacing: Maui.Style.space.big

            Item
            {
                id: _featuredSection
                Kirigami.Theme.inherit: false
                Kirigami.Theme.backgroundColor: "#333"
                Kirigami.Theme.textColor: "#fafafa"
                height: 250
                Layout.fillWidth: true
                Layout.margins: isWide ? Maui.Style.space.huge : Maui.Style.space.small

                Image
                {
                    id: _bannerImage
                    anchors.centerIn: parent
                    width: parent.width *2
                    height: parent.height * 2
                    sourceSize.height: height * 0.3
                    sourceSize.width: width * 0.3
                    source: _featuredListview.currentItem.imageSource
                    fillMode: Image.PreserveAspectCrop
                    antialiasing: false
                    smooth: false
                    asynchronous: true
                }

                FastBlur
                {
                    id: fastBlur
                    anchors.fill: parent
                    source: _bannerImage
                    radius: 64
                    transparentBorder: false
                    cached: true
                }

                Rectangle
                {
                    anchors.fill: parent
                    opacity: 0.7
                    color: Kirigami.Theme.backgroundColor
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
                            category: _categoriesList.baseCategory()
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
                                color: Kirigami.Theme.textColor
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
                            _featuredListview.currentIndex = index
                            _featureListBanner.setApp(model.id)
                            control.itemClicked(_featureListBanner.app)
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

                            label3.font.pointSize: Maui.Style.fontSizes.enormous
                            label3.font.bold: true
                            label3.font.weight: Font.Bold
                            label3.font.underline: true
                            label3.text: model.score
                            label4.text: i18n("Score")

                            iconVisible: isWide
                            rightLabels.visible: isWide
                            imageSource: model.preview
                            imageSizeHint: width * 0.3

                            imageBorder: false

                            fillMode: Image.PreserveAspectFit

                            leftLabels.data: Label
                            {
                                Layout.fillWidth: true
                                font.bold: true
                                font.weight: Font.Bold
                                color: Kirigami.Theme.textColor
                                text: model.typename + " - " + model.personid
                            }
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
                            anchors.fill: parent
                            radius: Maui.Style.radiusV * 2
                        }
                    }
                }
            }

            Maui.ListBrowser
            {
                id: _categoriesListView

                Layout.fillWidth: true
                implicitWidth: contentWidth
                implicitHeight: 160
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                Layout.margins: isWide ? Maui.Style.space.huge : Maui.Style.space.small
                snapMode: ListView.SnapOneItem
                orientation: Qt.Horizontal
                horizontalScrollBarPolicy: ScrollBar.AlwaysOff
                verticalScrollBarPolicy:  ScrollBar.AlwaysOff

                model: Maui.BaseModel
                {
                    id: _categoriesModel
                    list: _categoriesList
                }

                delegate: GridItemCard
                {
                    isCurrentItem: ListView.isCurrentItem
                    width: 100
                    height: 120
                    iconSource: model.icon
                    iconSizeHint: Maui.Style.iconSizes.big
                    label1.text: model.title

                    onClicked:
                    {
                        console.log("Category id", model.id)
                        _categoriesListView.currentIndex = index
                        _categoriesList.setCurrentCategory(_categoriesModel.get(index).id)
                        control.categoryClicked(_categoriesList.currentCategory)
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
                implicitHeight: leftLabels.implicitHeight
                label1.text: i18n("Newest")
                label1.font.pointSize: Maui.Style.fontSizes.enormous
                label1.font.bold: true
                label1.font.weight: Font.Bold
                label2.font.pointSize: Maui.Style.fontSizes.huge
                label2.text: i18n("Most newest additions to our collection.")
            }

            Maui.ListBrowser
            {
                id: _newestListView
                Layout.fillWidth: true
                Layout.margins: isWide ? Maui.Style.space.huge : Maui.Style.space.small

                implicitHeight: 120
                orientation: Qt.Horizontal
                spacing: Maui.Style.space.big
                horizontalScrollBarPolicy: ScrollBar.AlwaysOff
                snapMode: ListView.SnapOneItem
                verticalScrollBarPolicy: ScrollBar.AlwaysOff

                model: Maui.BaseModel
                {
                    list: NX.Store
                    {
                        id: _newestListModel
                        category: _categoriesList.baseCategory()
                        pageSize: 6
                        sort: NX.Store.NEWEST
                    }
                }

                delegate: FloatingCardDelegate
                {
                    width: Math.min(ListView.view.width * 0.9, 350)
                    height: 100

                    label1.text: model.name
                    label2.text: model.typename
                    label3.text: model.version

                    iconSource: model.smallpic
                    iconSizeHint: Maui.Style.iconSizes.huge

                    onClicked:
                    {
                        _newestListView.currentIndex = index
                        _newestListModel.setApp(model.id)
                        control.itemClicked(_newestListModel.app)
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
                implicitHeight: leftLabels.implicitHeight
                label1.text: i18n("Most Popular")
                label1.font.pointSize: Maui.Style.fontSizes.enormous
                label1.font.bold: true
                label1.font.weight: Font.Bold
                label2.font.pointSize: Maui.Style.fontSizes.huge
                label2.text: i18n("Most downloaded packages.")
            }

            Item{Layout.fillWidth: true}
        }

    }
}
