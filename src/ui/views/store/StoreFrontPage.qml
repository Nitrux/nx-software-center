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
    headBar.forceCenterMiddleContent: isWide
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
                imageSource: model.smallpic
                label1.text: model.name
                label2.text: model.typename
                label3.text: model.totaldownloads
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
            spacing: Maui.Style.space.huge

            FeatureHeader
            {
                id: _featureHeader
                Layout.fillWidth: true
                Layout.margins: isWide ? Maui.Style.space.huge : Maui.Style.space.small

                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                onAppClicked:
                {
                    control.itemClicked(app)
                }
            }

            FeatureStrip
            {
                id: _categoriesListView
                Layout.fillWidth: true
//                Layout.maximumWidth: Math.min( _featureGridView.flickable.width, contentWidth)
                listView.implicitHeight: 60
title.text: i18n("Categories")
subtitle.text: i18n("Filter by categories")
                model: Maui.BaseModel
                {
                    id: _categoriesModel
                    list: _categoriesList
                }

                delegate: Maui.ListBrowserDelegate
                {
                    width: template.implicitWidth + iconSizeHint
                    height: ListView.view.height

                    iconVisible: true
                    iconSource: model.icon
                    iconSizeHint: Maui.Style.iconSizes.medium
template.headerSizeHint: iconSizeHint * 2
                    label1.text: model.title
                    label1.font.bold: true
                    label1.font.weight: Font.Bold
                    label1.font.pointSize: Maui.Style.fontSizes.big

                    onDoubleClicked:
                    {
                        _categoriesListView.currentIndex = index
                        if(!Maui.Handy.singleClick)
                        {
                            control.categoryClicked(_categoriesList.getCategory(_categoriesModel.get(index).id))

                        }
                    }

                    onClicked:
                    {
                        _categoriesListView.currentIndex = index

                        if(Maui.Handy.singleClick || Kirigami.Settings.hasTransientTouchInput)
                        {
                            control.categoryClicked(_categoriesList.getCategory(_categoriesModel.get(index).id))

                        }
                    }
                }
            }

            Kirigami.Separator
            {
                Layout.fillWidth: true
                implicitHeight: 0.5
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
                subtitle.text: i18n("Most recent packages.")
                category: _categoriesList.baseCategory()
                pageSize: 6
                sort: NX.Store.NEWEST
                listView.implicitHeight: 80

                delegate: FloatingCardDelegate
                {
                    width: Math.min(ListView.view.width * 0.9, implicitWidth)
                    height: ListView.view.height

                    label1.text: model.name
                    label2.text: model.typename
                    imageSource: model.preview

                    onDoubleClicked:
                    {
                        _newestListView.currentIndex = index
                        if(!Maui.Handy.singleClick)
                        {
                            _newestListView.list.setApp(model.id)
                            control.itemClicked(_newestListView.list.app)
                        }
                    }

                    onClicked:
                    {
                        _newestListView.currentIndex = index
                        if(Maui.Handy.singleClick || Kirigami.Settings.hasTransientTouchInput)
                        {
                            _newestListView.list.setApp(model.id)
                            control.itemClicked(_newestListView.list.app)
                        }
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
                listView.implicitHeight: 80

                title.text: i18n("Popular in Audio")
                subtitle.text: i18n("Most popular Audio packages in our collection.")

                category: _categoriesList.audioCategory()
                pageSize: 6
                sort: NX.Store.MOST_DOWNLOADED

                delegate: FloatingCardDelegate
                {
                    width: Math.min(ListView.view.width * 0.9, implicitWidth)
                    height: ListView.view.height
                    label1.text: model.name
                    label2.text: model.typename
                    imageSource: model.smallpic

                    onDoubleClicked:
                    {
                        _popularAudioList.currentIndex = index
                        if(!Maui.Handy.singleClick)
                        {
                            _popularAudioList.list.setApp(model.id)
                            control.itemClicked(_popularAudioList.list.app)
                        }
                    }

                    onClicked:
                    {
                        _popularAudioList.currentIndex = index

                        if(Maui.Handy.singleClick|| Kirigami.Settings.hasTransientTouchInput)
                        {
                            _popularAudioList.list.setApp(model.id)
                            control.itemClicked(_popularAudioList.list.app)
                        }
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
