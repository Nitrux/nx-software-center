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
    title: _storeList.categoryName

    Kirigami.Theme.inherit: false
    Kirigami.Theme.colorGroup: Kirigami.Theme.View

    property var category
    property var tags

    signal itemClicked(var app)

    headBar.middleContent: [
        Maui.TextField
        {
            id: _searchField
            visible: control.tags.length === 0
            Layout.fillWidth: true
            Layout.maximumWidth: 500
            placeholderText: i18n("Search package in %1", _storeList.categoryName )
            onAccepted: search(text)
            onCleared: search("")
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

        onAtYEndChanged: //TODO if the list is at end beacuse there si not enough items to flick thend o not increase page
        {
            if(_listView.atYEnd && _listView.count> 0)
            {
                const prevPos = _listView.contentY
                _storeList.page ++

            }
        }

        BusyIndicator
        {
            anchors.centerIn: parent
            running: _listView.count === 0
        }

        flickable.header: ColumnLayout
        {
            width: parent.width
            height: visible ? implicitHeight : 0
            visible: _storeList.nameFilter === "" && control.tags.length === 0

            spacing: Maui.Style.space.huge

            Maui.ListBrowser
            {
                id: _categoriesListView

                visible: count > 0
                currentIndex: -1
                Layout.fillWidth: true
                implicitHeight: 80
                Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
                Layout.margins: isWide ? Maui.Style.space.big : Maui.Style.space.small
                snapMode: ListView.SnapOneItem
                orientation: Qt.Horizontal
                horizontalScrollBarPolicy: ScrollBar.AlwaysOff
                verticalScrollBarPolicy:  ScrollBar.AlwaysOff

                flickable.header: Item
                {
                    width : 180
                    height: 64

                    Maui.ItemDelegate
                    {
                        isCurrentItem: _categoriesListView.currentIndex === -1
                        width: 160
                        height: 64

                        anchors.centerIn: parent

                        background: Rectangle
                        {
                            opacity: 0.5
                            color: Qt.tint(control.Kirigami.Theme.textColor, Qt.rgba(control.Kirigami.Theme.backgroundColor.r, control.Kirigami.Theme.backgroundColor.g, control.Kirigami.Theme.backgroundColor.b, 0.9))
                            radius: Maui.Style.radiusV

                            Rectangle
                            {
                                visible: _categoriesListView.currentIndex === -1
                                width: parent.width
                                height: 8
                                color: Kirigami.Theme.textColor
                                anchors.bottom: parent.bottom
                            }
                        }

                        Maui.ListItemTemplate
                        {
                            anchors.fill: parent
                            label1.text: currentCategory.name
                            label1.horizontalAlignment: Qt.AlignHCenter
                            label1.font.pointSize: Maui.Style.fontSizes.big
                            label1.font.bold: true
                            label1.font.weight: Font.Bold
                        }

                        onDoubleClicked:
                        {
                            _categoriesListView.currentIndex = -1
                            if(!Maui.Handy.singleClick)
                            {
                                control.category = currentCategory
                            }
                        }

                        onClicked:
                        {
                            _categoriesListView.currentIndex = -1

                            if(Maui.Handy.singleClick || Kirigami.Settings.hasTransientTouchInput)
                            {
                                control.category = currentCategory
                            }
                        }
                    }
                }

                model: Maui.BaseModel
                {
                    id: _categoriesModel
                    list: NX.Categories
                    {
                        id: _subCategories
                        category: currentCategory
                    }
                }

                delegate: Maui.ItemDelegate
                {
                    isCurrentItem: ListView.isCurrentItem
                    width: Math.max(160, _template.implicitWidth)
                    height: 64

                    background: Rectangle
                    {
                        opacity: 0.5
                        color: Qt.tint(control.Kirigami.Theme.textColor, Qt.rgba(control.Kirigami.Theme.backgroundColor.r, control.Kirigami.Theme.backgroundColor.g, control.Kirigami.Theme.backgroundColor.b, 0.9))
                        radius: Maui.Style.radiusV

                        Rectangle
                        {
                            visible: hovered || isCurrentItem
                            width: parent.width
                            height: 8
                            color: Kirigami.Theme.textColor
                            anchors.bottom: parent.bottom
                        }
                    }

                    Maui.ListItemTemplate
                    {
                        id: _template
                        anchors.fill: parent
                        label1.text: model.title
                        label1.horizontalAlignment: Qt.AlignHCenter
                        label1.font.pointSize: Maui.Style.fontSizes.big
                        label1.font.bold: true
                        label1.font.weight: Font.Bold
                    }

                    onDoubleClicked:
                    {
                        _categoriesListView.currentIndex = index
                        if(!Maui.Handy.singleClick)
                        {
                            control.category = _subCategories.getCategory(_categoriesModel.get(_categoriesListView.currentIndex).id)
                        }
                    }

                    onClicked:
                    {
                        _categoriesListView.currentIndex = index

                        if(Maui.Handy.singleClick || Kirigami.Settings.hasTransientTouchInput)
                        {
                            control.category = _subCategories.getCategory(_categoriesModel.get(_categoriesListView.currentIndex).id)
                        }
                    }
                }
            }

            Maui.Separator
            {
                Layout.fillWidth: true
                edge: Qt.BottomEdge
            }

            FeatureStrip
            {
                id: _popularListView
                Layout.fillWidth: true
                title.text: i18n("Favorite in %1 ", control.category.displayName)
                subtitle.text: i18n("Hightest rated app packages.")

                category: control.category
                pageSize: 4
                sort: NX.Store.HIGHEST_RATED

                onAppClicked:
                {
                    control.itemClicked(app)
                }
            }

            FeatureStrip
            {
                id: _newestListView

                Layout.fillWidth: true
                title.text: i18n("Newest in %1 ", control.category.displayName)
                subtitle.text: i18n("Most newest additions to our collection.")

                listView.implicitHeight: 80

                category: control.category
                pageSize: 4
                sort: NX.Store.NEWEST

                delegate: FloatingCardDelegate
                {
                    width: Math.min(ListView.view.width * 0.9, 350)
                    height: ListView.view.height

                    label1.text: model.name
                    label2.text: model.typename
                    label3.text: model.version
                    iconSource: "package"
                    imageSource: model.smallpic
                    template.imageSizeHint: Maui.Style.iconSizes.huge
                    iconSizeHint: Maui.Style.iconSizes.medium

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

            Maui.Separator
            {
                Layout.fillWidth: true
                edge: Qt.BottomEdge
            }

            SectionTitle
            {
                label1.text: i18n("All in %1 ", control.category.displayName)
                label2.text: i18n("All packages under category.")
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
                tags: control.tags
                sort: NX.Store.ALPHABETICAL
            }
        }

        delegate: Maui.ListBrowserDelegate
        {
            id: _delegate
            height: Math.min(implicitHeight + Maui.Style.space.huge, 100)
            width: ListView.view.width

            label1.text: model.name
            label1.font.pointSize: Maui.Style.fontSizes.big
            label1.font.bold: true
            label1.font.weight: Font.Bold
            label2.text: model.typename
            label3.text: model.score
            label3.font.bold: true
            label3.font.weight: Font.Black
            label3.font.pointSize: 16
            label4.text:i18n("Score")
            imageSource: model.smallpic
            iconVisible: true
            template.imageSizeHint: Maui.Style.iconSizes.huge
            iconSource: "package"
            iconSizeHint:  Maui.Style.iconSizes.medium

            onClicked:
            {
                _listView.currentIndex = index
                if(Maui.Handy.singleClick)
                {
                    _storeList.setApp(model.id)
                    control.itemClicked(_storeList.app)
                }
            }

            onDoubleClicked:
            {
                _listView.currentIndex = index
                if(!Maui.Handy.singleClick)
                {
                    _storeList.setApp(model.id)
                    control.itemClicked(_storeList.app)
                }
            }
        }
    }

    function search(query)
    {
        _searchField.text = query
        _storeList.nameFilter = query
    }
}
