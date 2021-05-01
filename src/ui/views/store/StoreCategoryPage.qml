import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import org.kde.kirigami 2.14 as Kirigami
import org.mauikit.controls 1.2 as Maui

import NXModels 1.0 as NX

import "../../templates"

Maui.Page
{
    id: control

    property var category

    signal itemClicked(var app)

    //    headBar.visible: true
    headBar.middleContent: [
        Maui.TextField
        {
            id: _searchField
            Layout.fillWidth: true
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

        flickable.header: ColumnLayout
        {
            width: parent.width
            height: visible ? implicitHeight : 0
            visible: _storeList.nameFilter === ""

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

                        onClicked:
                        {
                            _categoriesListView.currentIndex = -1
                            control.category = currentCategory
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

                    onClicked:
                    {
                        console.log("Category id", model.id)
                        _categoriesListView.currentIndex = index
                        control.category = _subCategories.getCategory(_categoriesModel.get(_categoriesListView.currentIndex).id)
                    }
                }
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

                listView.implicitHeight: 120

                category: control.category
                pageSize: 4
                sort: NX.Store.NEWEST

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
                        _newestListView.list.setApp(model.id)
                        control.itemClicked(_newestListView.list.app)
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
                sort: NX.Store.ALPHABETICAL
            }
        }

        delegate: Maui.ItemDelegate
        {
            id: _delegate
            height: Math.min(template.implicitHeight + Maui.Style.space.huge, 100)
            width: ListView.view.width

            Maui.ListItemTemplate
            {
                id: template
                anchors.fill: parent
                label1.text: model.name
                label1.font.pointSize: Maui.Style.fontSizes.big
                label1.font.bold: true
                label1.font.weight: Font.Bold
                label2.text: model.typename
                //                    label4.text: model.score + qsTr(" Points")
                iconSource: model.smallpic
                iconVisible: true
                iconSizeHint:  Maui.Style.iconSizes.large

                Maui.GridItemTemplate
                {
                    implicitWidth: 64
                    implicitHeight: 48
                    iconComponent: Rectangle
                    {
                        Layout.preferredWidth: 48
                        radius: Maui.Style.radiusV
                        color: Qt.tint(Kirigami.Theme.textColor, Qt.rgba(Kirigami.Theme.backgroundColor.r, Kirigami.Theme.backgroundColor.g, Kirigami.Theme.backgroundColor.b, 0.9))

                        Label
                        {
                            anchors.fill: parent
                            horizontalAlignment: Qt.AlignHCenter
                            text: model.score
                            font.bold: true
                            font.weight: Font.Black
                            font.pointSize: 16
                        }
                    }

                    label1.text: i18n("Score")
                }
            }

            onClicked:
            {
                _listView.currentIndex = index
                _storeList.setApp(model.id)
                control.itemClicked(_storeList.app)
            }
        }
    }

    function search(query)
    {
        _searchField.text = query
        _storeList.nameFilter = query
    }
}
