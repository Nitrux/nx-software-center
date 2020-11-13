import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import org.kde.kirigami 2.14 as Kirigami
import org.kde.mauikit 1.2 as Maui

import NXModels 1.0 as NX

Maui.Page
{
    id: control

    property alias category : _storeList.category

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

        model: Maui.BaseModel
        {
            id: _appsModel
            list: NX.Store
            {
                id: _storeList
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
                control.push(_appPageComponent)
                _storeList.setApp(_listView.model.get(_listView.currentIndex).id)
                control.currentItem.data = _storeList.app
            }
        }
    }

}
