import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import org.mauikit.controls 1.3 as Maui

import NXModels 1.0 as NX

import "../../templates"


SectionTitle
{
    id: control

    spacing: Maui.Style.space.big

    property alias listView : _listView
    property alias model :  _listView.model
    property alias list : _appsList
    property alias category: _appsList.category
    property alias pageSize : _appsList.pageSize
    property alias sort :_appsList.sort
    property alias subtitle : control.description
    property alias delegate : _listView.delegate

    property alias currentIndex: _listView.currentIndex

    signal appClicked(var app)

    ListView
    {
        id: _listView
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignVCenter

        cacheBuffer: contentWidth
        implicitHeight: isWide && !Maui.Handy.isMobile ? 260 : 180
        orientation: Qt.Horizontal
        spacing: Maui.Style.space.huge
        snapMode: ListView.NoSnap
        boundsMovement: Flickable.StopAtBounds
        currentIndex : -1

        BusyIndicator
        {
            anchors.centerIn: parent
            running: _listView.count === 0
        }

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

            width: Math.min(ListView.view.width* 0.6, 320)
            height: ListView.view.height
            //            margins: Maui.Style.space.tiny
            label1.text: model.name
            label2.text: model.typename
            imageSource: model.preview
            label3.text: model.score

            NX.App
            {
                id: _app
                data: _appsList.application(model.id)
            }

            onDoubleClicked:
            {
                _listView.currentIndex = index
                if(!Maui.Handy.singleClick)
                {
                    _appsList.setApp(model.id)
                    control.appClicked(_appsList.app)
                }
            }

            onClicked:
            {
                _listView.currentIndex = index

                if(Maui.Handy.singleClick || Maui.Handy.hasTransientTouchInput)
                {
                    _appsList.setApp(model.id)
                    control.appClicked(_appsList.app)
                }
            }
        }

        AbstractButton
        {
            id: _rightHandle
            visible: !Maui.Handy.isTouch && enabled
            implicitHeight: visible ? Maui.Style.iconSizes.big : 0
            implicitWidth: height

            anchors.right: parent.right
            anchors.verticalCenter: parent.verticalCenter

            enabled: !_listView.atXEnd
            background: Rectangle
            {
                radius: height* 0.5
                color: _rightHandle.pressed ? Maui.Theme.highlightColor : Maui.Theme.backgroundColor
            }

            onClicked: _listView.contentX= _listView.contentX+300

            contentItem: Item
            {
                Maui.Icon
                {
                    anchors.centerIn: parent
                    height: Maui.Style.iconSizes.medium
                    width: height
                    source: "go-next"
                }
            }
        }

        AbstractButton
        {
            id: _leftHandle
            visible: !Maui.Handy.isTouch && enabled
            implicitHeight: visible ? Maui.Style.iconSizes.big : 0
            implicitWidth: height
            anchors.left: parent.left
            anchors.verticalCenter: parent.verticalCenter
            enabled: !_listView.atXBeginning

            background: Rectangle
            {
                radius: height* 0.5
                color: _leftHandle.pressed ? Maui.Theme.highlightColor : Maui.Theme.backgroundColor
            }

            onClicked: _listView.contentX= _listView.contentX-300

            contentItem: Item
            {
                Maui.Icon
                {
                    anchors.centerIn: parent
                    height: Maui.Style.iconSizes.medium
                    width: height
                    source: "go-previous"
                }
            }
        }
    }
}

