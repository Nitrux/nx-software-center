import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import org.kde.kirigami 2.14 as Kirigami
import org.kde.mauikit 1.2 as Maui

import NXModels 1.0 as NX

import "../../templates"

Maui.Page
{
    id: control

    property var category

    signal itemClicked(var app)

    headBar.middleContent: [
        Maui.TextField
        {
            Layout.fillWidth: true
            placeholderText: i18n("Search package in %1", _storeList.categoryName )
            onTextChanged: _storeList.nameFilter = text
        }
    ]

    headBar.rightContent: Maui.ToolButtonMenu
    {
        icon.name: "view-sort"
        MenuItem
        {
            text: qsTr("Newest")
            onTriggered: _storeList.sort = NX.Store.NEWEST
            checked: _storeList.sort === NX.Store.NEWEST
            checkable: true
            autoExclusive: true
        }

        MenuItem
        {
            text: qsTr("A-Z")
            onTriggered: _storeList.sort = NX.Store.ALPHABETICAL
            checked: _storeList.sort === NX.Store.ALPHABETICAL
            checkable: true
            autoExclusive: true
        }

        MenuItem
        {
            text: qsTr("Highest Rated")
            onTriggered: _storeList.sort = NX.Store.HIGHEST_RATED
            checked: _storeList.sort === NX.Store.HIGHEST_RATED
            checkable: true
            autoExclusive: true
        }

        MenuItem
        {
            text: qsTr("Most Downloaded")
            onTriggered: _storeList.sort = NX.Store.MOST_DOWNLOADED
            checked: _storeList.sort === NX.Store.MOST_DOWNLOADED
            checkable: true
            autoExclusive: true
        }
    }

    Maui.ListBrowser
    {
        id: _listView
        anchors.fill: parent
        orientation: ListView.Vertical
        spacing: Maui.Style.space.big

        onAtYEndChanged:
        {
            if(_listView.atYEnd)
            {
                const prevPos = _listView.contentY
                _storeList.page ++

            }
        }

        flickable.header: ColumnLayout
        {
            width: _listView.width

            Maui.ListBrowser
            {
                id: _categoriesListView

                Layout.fillWidth: true
                implicitHeight: 120
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                Layout.margins: isWide ? Maui.Style.space.huge : Maui.Style.space.small
                snapMode: ListView.SnapOneItem
                orientation: Qt.Horizontal
                horizontalScrollBarPolicy: ScrollBar.AlwaysOff
                verticalScrollBarPolicy:  ScrollBar.AlwaysOff

                model: Maui.BaseModel
                {
                    id: _categoriesModel
                    list: NX.Categories
                    {
                        category: control.category
                    }
                }

                delegate: Maui.ItemDelegate
                {
                    isCurrentItem: ListView.isCurrentItem
                    width: 160
                    height: 64

                    background:Rectangle
                    {
                        opacity: 0.5
                        color: Qt.tint(control.Kirigami.Theme.textColor, Qt.rgba(control.Kirigami.Theme.backgroundColor.r, control.Kirigami.Theme.backgroundColor.g, control.Kirigami.Theme.backgroundColor.b, 0.9))
                        radius: Maui.Style.radiusV
                    }

                    Maui.ListItemTemplate
                    {
                        anchors.fill: parent
                        label1.text: model.title
                        label1.horizontalAlignment: Qt.AlignHCenter
                        label1.font.pointSize: Maui.Style.fontSizes.big
                        label1.font.bold: true
                        label1.font.weight: Font.Bold
                    }

                    onClicked:
                    {
                        console.log("Category id", model.id)
                        _categoriesListView.currentIndex = index
                        _categoriesList.setCurrentCategory(_categoriesModel.get(index).id)
                        control.categoryClicked(_categoriesList.currentCategory)
                    }
                }
            }

            SectionTitle
            {
                label1.text: i18n("Newest in %1 ", control.category.displayName)
                label2.text: i18n("Most newest additions to our collection.")
            }

            Maui.ListBrowser
            {
                id: _newestListView

                Layout.fillWidth: true
                Layout.margins: isWide ? Maui.Style.space.huge : Maui.Style.space.small
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter

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
                        category: control.category
                        pageSize: 4
                        sort: NX.Store.MOST_DOWNLOADED
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

            SectionTitle
            {
                label1.text: control.category.displayName
                label2.text: i18n("Packages under category.")
            }


            Item
            {
                Layout.fillWidth: true
                Layout.margins: Maui.Style.space.medium
            }
        }

        model: Maui.BaseModel
        {
            id: _appsModel
            list: NX.Store
            {
                id: _storeList
                category: control.category
                sort: NX.Store.ALPHABETICAL
            }
        }

        delegate: Maui.SwipeBrowserDelegate
        {
            id: _delegate
            height: Math.min(template.leftLabels.implicitHeight + Maui.Style.space.huge, 100)
            width: ListView.view.width
            label1.text: model.name
            label1.font.pointSize: Maui.Style.fontSizes.big
            label1.font.bold: true
            label1.font.weight: Font.Bold
            //                    label2.wrapMode: Text.WordWrap
            //                    label2.text: model.description.slice(0, Math.min(model.description.length, 100))
            label2.text: model.totaldownloads + qsTr(" Downloads") + "\n" + model.score + qsTr(" Points")
            label3.text: model.typename
            //                    label4.text: model.score + qsTr(" Points")
            iconSource: model.smallpic
            iconVisible: true
            iconSizeHint:  Maui.Style.iconSizes.large

            quickActions: [

                Action
                {
                    icon.name: "media-playback-start"
                },

                Action
                {
                    icon.name: "entry-delete"
                },

                Action
                {
                    icon.name: "download"
                }
            ]

            onClicked:
            {
                _listView.currentIndex = index
                _storeList.setApp(model.id)
                control.itemClicked(_storeList.app)
            }
        }
    }

}
