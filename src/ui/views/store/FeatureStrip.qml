import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import org.kde.kirigami 2.14 as Kirigami
import org.mauikit.controls 1.2 as Maui

import NXModels 1.0 as NX

import "../../templates"

ColumnLayout
{
    id: control
//    visible: list.count > 0
    spacing: Maui.Style.space.small

    property alias listView : _listView
    property alias model :  _listView.model
    property alias list : _appsList
    property alias category: _appsList.category
    property alias pageSize : _appsList.pageSize
    property alias sort :_appsList.sort
    property alias title : _section.label1
    property alias subtitle : _section.label2
    property alias delegate : _listView.delegate

    property alias currentIndex: _listView.currentIndex

    signal appClicked(var app)

    SectionTitle
    {
        id: _section
        label3.text: list.count
    }

    Maui.ListBrowser
    {
        id: _listView
        Layout.fillWidth: true
        Layout.margins: isWide ? Maui.Style.space.medium : Maui.Style.space.small
        Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
        cacheBuffer: contentWidth
        implicitHeight: 220
        orientation: Qt.Horizontal
        spacing: Maui.Style.space.big
        horizontalScrollBarPolicy: ScrollBar.AlwaysOff
        snapMode: ListView.SnapOneItem
        verticalScrollBarPolicy: ScrollBar.AlwaysOff

        model: Maui.BaseModel
        {
            list: NX.Store
            {
                id: _appsList
            }
        }

        delegate: FeatureGridCard
        {
            images: _app.images

            width: Math.min(ListView.view.width, 320)
            height: 200
//            margins: Maui.Style.space.tiny
            label1.text: model.name
            label2.text: model.typename
            label3.text: model.totaldownloads
            label4.text: model.personid

            NX.App
            {
                id: _app
                data: _appsList.application(model.id)
            }

            onClicked:
            {
                _listView.currentIndex = index
                _appsList.setApp(model.id)
                control.appClicked(_appsList.app)
            }
        }


        Rectangle
        {
            id: _leftHandle
            visible: !_listView.atXBeginning && _listView.contentWidth > _listView.width
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter

            height: Maui.Style.iconSizes.big
            width: height

            radius: height* 0.5

            color: Kirigami.Theme.backgroundColor

            MouseArea
            {
                anchors.fill: parent
                onClicked: _listView.flickable.decrementCurrentIndex()
            }

            Kirigami.Icon
            {
                anchors.centerIn: parent
                height: Maui.Style.iconSizes.medium
                width: height
                source: "go-previous"
            }
        }

        Rectangle
        {
            id: _rightHandle
            visible: !_listView.atXEnd && _listView.contentWidth > _listView.width

            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            height: Maui.Style.iconSizes.big
            width: height

            radius: height* 0.5

            color: Kirigami.Theme.backgroundColor

            MouseArea
            {
                anchors.fill: parent
                onClicked: _listView.flickable.incrementCurrentIndex()
            }

            Kirigami.Icon
            {
                anchors.centerIn: parent
                height: Maui.Style.iconSizes.medium
                width: height
                source: "go-next"
            }
        }

        DropShadow
        {

            anchors.fill: _leftHandle
            cached: true
            horizontalOffset: 0
            verticalOffset: 0
            radius: 8.0
            samples: 16
            color: "#333"
            smooth: true
            source: _leftHandle
        }

        DropShadow
        {

            anchors.fill: _rightHandle
            cached: true
            horizontalOffset: 0
            verticalOffset: 0
            radius: 8.0
            samples: 16
            color: "#333"
            smooth: true
            source: _rightHandle
        }

    }

}
