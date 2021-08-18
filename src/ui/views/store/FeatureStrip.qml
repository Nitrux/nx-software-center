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
    }

    RowLayout
    {
        Layout.fillWidth: true
        spacing: Maui.Style.space.medium
        //        Layout.margins: isWide ? Maui.Style.space.medium : Maui.Style.space.small

        AbstractButton
        {
            id: _leftHandle
            visible: !Maui.Handy.isTouch
            z: _listView.z +1
            implicitHeight: Maui.Style.iconSizes.big
            implicitWidth: height
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
            enabled: !_listView.atXBeginning

            background: Rectangle
            {
                radius: height* 0.5
                color: _leftHandle.pressed ? Kirigami.Theme.highlightColor : Kirigami.Theme.backgroundColor
            }

            onClicked: _listView.contentX= _listView.contentX-300

            contentItem: Item
            {
                Kirigami.Icon
                {
                    anchors.centerIn: parent
                    height: Maui.Style.iconSizes.medium
                    width: height
                    source: "go-previous"
                }
            }
        }

        ListView
        {
            id: _listView
            Layout.fillWidth: true

            cacheBuffer: contentWidth
            implicitHeight: 220
            orientation: Qt.Horizontal
            spacing: Maui.Style.space.big
            //        horizontalScrollBarPolicy: ScrollBar.AlwaysOff
            snapMode: ListView.SnapToItem
            //        verticalScrollBarPolicy: ScrollBar.AlwaysOff
            boundsMovement: Flickable.StopAtBounds
//            boundsBehavior: Flickable.DragAndOvershootBounds
//            transform: Rotation {
//                axis { x: 0; y: 1; z: 0 }
//                origin.x: _listView.width / 2
//                origin.y: _listView.height / 2
//                angle: Math.min(30, Math.max(-30, _listView.horizontalOvershoot))
//            }
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
                height: ListView.view.height * 0.9
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

                onClicked:
                {
                    _listView.currentIndex = index
                    _appsList.setApp(model.id)
                    control.appClicked(_appsList.app)
                }
            }
        }

        AbstractButton
        {
            id: _rightHandle
visible: !Maui.Handy.isTouch
implicitHeight: Maui.Style.iconSizes.big
            implicitWidth: height
            Layout.alignment: Qt.AlignVCenter | Qt.AlignHCenter
enabled: !_listView.atXEnd
            background: Rectangle
            {
                radius: height* 0.5
                color: _rightHandle.pressed ? Kirigami.Theme.highlightColor : Kirigami.Theme.backgroundColor
            }

            onClicked: _listView.contentX= _listView.contentX+300

            contentItem: Item
            {
                Kirigami.Icon
                {
                    anchors.centerIn: parent
                    height: Maui.Style.iconSizes.medium
                    width: height
                    source: "go-next"
                }
            }
        }

    }

}
