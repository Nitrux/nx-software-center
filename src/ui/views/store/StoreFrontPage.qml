import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3

import org.mauikit.controls 1.3 as Maui

import NXModels 1.0 as NX

import "../../templates"

Maui.SideBarView
{
    id: control

    signal itemClicked(var app)
    signal categoryClicked(var category)
    signal searchFor(var query)

    sideBar.preferredWidth: Maui.Style.units.gridUnit * 12
    sideBar.resizeable: false
    sideBarContent: Pane
    {
        Maui.Theme.colorSet: Maui.Theme.Window
        anchors.fill: parent

        padding: Maui.Style.space.medium

        ScrollView
        {
            anchors.fill: parent

            Flickable
            {
                contentHeight: _sideBarLayout.implicitHeight

                Column
                {
                    id: _sideBarLayout
                    width: parent.width

                    spacing: Maui.Style.space.big

                    Maui.SettingsSection
                    {
                        title: i18n("Categories")
                        width: parent.width

                        GridLayout
                        {
                            Layout.fillWidth: true

                            columns: 3
                            columnSpacing: Maui.Style.space.small
                            rowSpacing: Maui.Style.space.small

                            Repeater
                            {
                                model: Maui.BaseModel
                                {
                                    id: _categoriesModel
                                    list: _categoriesList
                                }

                                delegate:  Item
                                {
                                    Layout.preferredHeight: Math.min(50, width)
//                                    Layout.preferredWidth: 50
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true

                                    Maui.GridBrowserDelegate
                                    {
                                        id: _categoryDelegate
                                        anchors.fill: parent
                                        property color tagColor : model.color
                                        template.isMask: true
                                        iconVisible: true
                                        iconSource: model.icon
                                        iconSizeHint: Maui.Style.iconSize
                                        tooltipText: model.title


                                        onDoubleClicked:
                                        {
                                            if(!Maui.Handy.singleClick)
                                            {
                                                control.categoryClicked(_categoriesList.getCategory(_categoriesModel.get(index).id))

                                            }
                                        }

                                        onClicked:
                                        {
                                            if(Maui.Handy.singleClick || Maui.handy.hasTransientTouchInput)
                                            {
                                                control.categoryClicked(_categoriesList.getCategory(_categoriesModel.get(index).id))

                                            }
                                        }

                                        //                                        background: Rectangle
                                        //                                        {
                                        //                                            readonly property color m_color : Qt.tint(model.color, Qt.rgba(Maui.Theme.alternateBackgroundColor.r, Maui.Theme.alternateBackgroundColor.g, Maui.Theme.alternateBackgroundColor.b, 0.9))

                                        //                                            color: _categoryDelegate.hovered || _categoryDelegate.containsPress ? Maui.Theme.hoverColor : m_color
                                        //                                            radius: Maui.Style.radiusV

                                        //                                        }
                                    }
                                }
                            }
                        }
                    }


                    Column
                    {
                        width: parent.width
                        spacing: Maui.Style.space.medium
                        Maui.ListBrowserDelegate
                        {
                            width: parent.width
                            label1.text: i18n("Newest")
                            iconSource: "draw-star"
                            iconSizeHint: Maui.Style.iconSize
                            template.isMask: true
                        }

                        Maui.ListBrowserDelegate
                        {
                            width: parent.width
                            label1.text: i18n("Hot")
                            iconSource: "love"
                            iconSizeHint: Maui.Style.iconSize
                            template.isMask: true
                        }

                        Maui.ListBrowserDelegate
                        {
                            width: parent.width
                            label1.text: i18n("Our picks")
                            iconSource: "alarm"
                            iconSizeHint: Maui.Style.iconSize
                            template.isMask: true
                        }
                    }

                    Maui.SettingsSection
                    {
                        title: i18n("Tags")
//                        description : i18n("Common tags")
                        width: parent.width
                        Flow
                        {
                            id: _tags
                            Layout.fillWidth: true
                            Repeater
                            {
                                model: ["Convergent", "Mobile", "ARM", "x86", "Maui", "Email", "Android"]

                                delegate: Maui.Chip
                                {

                                    label.text: modelData
                                    iconSource: "tag"
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    Maui.Page
    {
        anchors.fill: parent
        //        headBar.background: null
        //        headBar.visible: control.sideBar.collapsed
        headBar.leftContent: ToolButton
        {
            checked: control.sideBar.visible
            icon.name: checked ? "sidebar-collapse" : "sidebar-expand"
            //            text: i18n("Browser")
            onClicked: control.sideBar.toggle()
        }

        headBar.visible: true
        //        headBar.background: null
        headBar.forceCenterMiddleContent: false
        headBar.middleContent: Maui.SearchField
        {
            Layout.fillWidth: true
            Layout.maximumWidth: 500
            Layout.alignment: Qt.AlignCenter
            placeholderText: i18n ("Search")
            onAccepted: control.searchFor(text)
        }

        Maui.GridView
        {
            id: _featureGridView
            clip: false
            anchors.fill: parent
            itemSize: Math.floor(Math.min(320, Math.max(200, control.width * 0.3)))
            itemHeight: 200
            padding: Maui.Handy.isMobile ? Maui.Style.space.medium : Maui.Style.space.big

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
                    anchors.margins: Maui.Style.space.big

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
                spacing: Maui.Style.space.huge *2

                FeatureHeader
                {
                    id: _featureHeader
                    Layout.fillWidth: true
                    //                Layout.margins: isWide ? Maui.Style.space.huge : Maui.Style.space.small

                    Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                    onAppClicked:
                    {
                        control.itemClicked(app)
                    }
                }

                Maui.Separator
                {
                    Layout.fillWidth: true
                    implicitHeight: 0.5
                }

                FeatureStrip
                {
                    Layout.fillWidth: true
                    title: i18n("Maui Apps")
                    subtitle: i18n("Convergent applications for desktop and mobile computers made with MauiKit.")

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
                    title: i18n("Newest")
                    subtitle: i18n("Most recent packages.")
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
                            if(Maui.Handy.singleClick || Maui.Handy.hasTransientTouchInput)
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
                    title: i18n("Popular in Games")
                    subtitle: i18n("Most popular Games packages in our collection.")

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

                    title: i18n("Popular in Audio")
                    subtitle: i18n("Most popular Audio packages in our collection.")

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

                            if(Maui.Handy.singleClick|| Maui.Handy.hasTransientTouchInput)
                            {
                                _popularAudioList.list.setApp(model.id)
                                control.itemClicked(_popularAudioList.list.app)
                            }
                        }
                    }
                }

                SectionTitle
                {
                    title: i18n("Most Popular")
                    description: i18n("Most downloaded packages.")
                }
            }
        }
    }
}
