import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3

import org.kde.kirigami 2.14 as Kirigami
import org.mauikit.controls 1.2 as Maui

import NXModels 1.0 as NX

import "../../templates"

Maui.Page
{
    id: control

    signal itemClicked(var app)
    signal categoryClicked(var category)
    signal searchFor(var query)

    Kirigami.Theme.colorSet: Kirigami.Theme.View
    Kirigami.Theme.inherit: false

    headBar.visible: true
    headBar.middleContent: Maui.TextField
    {
        Layout.fillWidth: true
        Layout.maximumWidth: 500
        placeholderText: i18n ("Search your app package...")
        onAccepted: control.searchFor(text)
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

        delegate: Item
        {
            width: GridView.view.cellWidth
            height: GridView.view.cellHeight

            FeatureGridCard
            {
                anchors.fill: parent
                anchors.margins: Maui.Style.space.medium

                images: _app.images

                label1.text: model.name
                label2.text: model.typename
                //            label3.text: model.totaldownloads
                //            label4.text: model.personid

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
        }

        flickable.header: ColumnLayout
        {
            width: _featureGridView.flickable.width
            spacing: Maui.Style.space.big

            FeatureHeader
            {
                id: _featureHeader
                Layout.fillWidth: true
                Layout.margins: isWide ? Maui.Style.space.huge : Maui.Style.space.small
                Layout.maximumWidth: 600
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                onAppClicked:
                {
                    control.itemClicked(app)
                }
            }

        Maui.ListBrowser
            {
                id: _categoriesListView

                Layout.fillWidth: true
                Layout.maximumWidth: Math.min( _featureGridView.flickable.width, contentWidth)
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
                        control.categoryClicked(_categoriesList.getCategory(_categoriesModel.get(index).id))
                    }
                }
            }

        FeatureStrip
        {
            Layout.fillWidth: true
            title.text: i18n("Maui Apps")
            subtitle.text: i18n("Convergent applications for desktop and mobile computers made with MauiKit.")

            category: _categoriesList.baseCategory()
            pageSize: 6
            sort: NX.Store.MOST_DOWNLOADED
            list.tags: ["mauikit"]

            listView.implicitHeight: 320
            onAppClicked:
            {
                control.itemClicked(app)
            }
        }

            FeatureStrip
            {
                id: _newestListView

                Layout.fillWidth: true
                title.text: i18n("Newest")
                subtitle.text: i18n("Most newest additions to our collection.")
                category: _categoriesList.baseCategory()
                pageSize: 6
                sort: NX.Store.NEWEST
                listView.implicitHeight: 120

                delegate: FloatingCardDelegate
                {
                    width: Math.min(ListView.view.width * 0.9, implicitWidth)
                    height: 100

                    label1.text: model.name
                    label2.text: model.typename
                    label3.text: model.version

                    iconSource: model.smallpic
                    iconSizeHint: Maui.Style.iconSizes.huge

                    onClicked:
                    {
                        _newestListView.currentIndex = index
                        _newestListView.list.setApp(model.id)
                        control.itemClicked(_newestListView.list.app)
                    }
                }
            }

            FeatureStrip
            {
                Layout.fillWidth: true
                title.text: i18n("Popular in Games")
                subtitle.text: i18n("Most popular Games packages in our collection.")

                category: _categoriesList.gamesCategory()
                pageSize: 6
                sort: NX.Store.MOST_DOWNLOADED

                onAppClicked:
                {
                    control.itemClicked(app)
                }
            }


            FeatureStrip
            {
                id: _popularAudioList
                Layout.fillWidth: true
                listView.implicitHeight: 120

                title.text: i18n("Popular in Audio")
                subtitle.text: i18n("Most popular Audio packages in our collection.")

                category: _categoriesList.audioCategory()
                pageSize: 6
                sort: NX.Store.MOST_DOWNLOADED

                delegate: FloatingCardDelegate
                {
                    width: Math.min(ListView.view.width * 0.9, implicitWidth)
                    height: 100

                    label1.text: model.name
                    label2.text: model.typename
                    label3.text: model.version

                    iconSource: model.smallpic
                    iconSizeHint: Maui.Style.iconSizes.huge

                    onClicked:
                    {
                        _popularAudioList.currentIndex = index
                        _popularAudioList.list.setApp(model.id)
                        control.itemClicked(_popularAudioList.list.app)
                    }
                }
            }

            SectionTitle
            {
                label1.text: i18n("Most Popular")
                label2.text: i18n("Most downloaded packages.")
            }
        }
    }
}
