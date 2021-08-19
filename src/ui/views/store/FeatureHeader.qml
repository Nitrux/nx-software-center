import QtQuick 2.14
import QtQuick.Controls 2.14

import QtGraphicalEffects 1.0

import org.kde.kirigami 2.14 as Kirigami
import org.mauikit.controls 1.2 as Maui

import NXModels 1.0 as NX

ListView
{
    id: control

    property int radius:  Maui.Style.radiusV * 2
    signal appClicked(var app)

    Kirigami.Theme.inherit: false
    Kirigami.Theme.backgroundColor: "#333"
    Kirigami.Theme.textColor: "#fafafa"
    implicitHeight: 250
    spacing: Maui.Style.space.big

    orientation: ListView.Horizontal
    snapMode: ListView.SnapOneItem
    clip: true
    highlightMoveDuration: 1000
    highlightMoveVelocity: -1

    onMovementEnded: currentIndex = indexAt(contentX, contentY)

    model: Maui.BaseModel
    {
        list: NX.Store
        {
            id: _featureListBanner
            category: _categoriesList.baseCategory()
            pageSize: 4
            sort: NX.Store.HIGHEST_RATED
        }
    }

    Timer
    {
        id: _featuredListviewTimer
        interval: 8000
        repeat: true
        running: !_featureHover.hovered
        onTriggered: control.cycleSlideForward()
    }

    Row
    {
        spacing: Maui.Style.space.medium
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.margins: Maui.Style.space.big

        Repeater
        {
            model: _featureListBanner.count

            Rectangle
            {
                width: Maui.Style.iconSizes.tiny
                height: width
                radius: width
                color: Kirigami.Theme.textColor
                opacity: index === control.currentIndex ? 1 : 0.5
            }
        }
    }

    delegate: Kirigami.ShadowedRectangle
    {

        width: Math.min(ListView.view.width , 600)
        height: ListView.view.height

        color: Qt.darker(Kirigami.Theme.backgroundColor)

        corners
        {
            topLeftRadius: control.radius
            topRightRadius: control.radius
            bottomLeftRadius: control.radius
            bottomRightRadius: control.radius
        }

        shadow.xOffset: 0
        shadow.yOffset: 0
        shadow.color: Qt.rgba(0, 0, 0, 0.3)
        shadow.size: 8

        Item
        {
            anchors.fill: parent
            opacity: 0.3
            clip: true
            Image
            {
                id: _bannerImage

                anchors.centerIn: parent
                width: parent.width *3
                height: parent.height * 3
                sourceSize.height: 64
                sourceSize.width: 64
                source: model.smallpic
                fillMode: Image.PreserveAspectCrop
                antialiasing: false
                smooth: false
                asynchronous: true
                rotation: 150
            }

            FastBlur
            {
                id: fastBlur
                anchors.fill: parent
                source: _bannerImage
                radius: 64
                transparentBorder: false
                cached: true
            }

            layer.enabled: true
            layer.effect: OpacityMask
            {
                maskSource: Item
                {
                    width: control.width
                    height: control.height

                    Rectangle
                    {
                        anchors.fill: parent
                        radius: control.radius
                    }
                }
            }
        }

        Maui.ListItemTemplate
        {
            //                            anchors.fill: parent
            anchors.centerIn: parent
            width: Math.min(parent.width*0.8, parent.width)
            //                            anchors.margins: Maui.Style.space.huge

            label1.font.pointSize: Maui.Style.fontSizes.enormous * 2
            label1.font.bold: true
            label1.font.weight: Font.Bold
            label1.verticalAlignment: Qt.AlignVCenter

            label1.text: model.name
            label2.text:  model.typename + " - " + model.personid
            //                            label2.wrapMode: Text.WordWrap
            //                            label2.verticalAlignment: Qt.AlignTop

            iconVisible: isWide
            imageSizeHint: Maui.Style.iconSizes.huge
            headerSizeHint: imageSizeHint * 1.5
            imageSource: model.preview
            leftLabels.spacing: Maui.Style.space.medium
            leftLabels.data: Button
            {
                text: i18n("View")
                z: 999
                onClicked:
                {
                    control.currentIndex = index
                    _featureListBanner.setApp(model.id)
                    control.appClicked(_featureListBanner.app)
                }
            }

            fillMode: Image.PreserveAspectFit

        }





    }

    HoverHandler
    {
        id: _featureHover
        target: control
    }
    function cycleSlideForward() {
        _featuredListviewTimer.restart();

        if (control.currentIndex === control.count - 1) {
            control.currentIndex = 0;
        } else {
            control.incrementCurrentIndex();
        }
    }

    function cycleSlideBackward() {
        _featuredListviewTimer.restart();

        if (control.currentIndex === 0) {
            control.currentIndex = control.count - 1;
        } else {
            control.decrementCurrentIndex();
        }
    }
}

