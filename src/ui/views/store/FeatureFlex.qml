import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import org.kde.kirigami 2.14 as Kirigami
import org.mauikit.controls 1.2 as Maui

import NXModels 1.0 as NX


ColumnLayout
{
    id: control

    property int radius:  Maui.Style.radiusV * 2
    signal appClicked(var app)

    Maui.Theme.inherit: false
    Maui.Theme.backgroundColor: "#333"
    Maui.Theme.textColor: "#fafafa"

    spacing: Maui.Style.space.medium

    Maui.ListItemTemplate
    {
        Layout.fillWidth: true

        label1.text: i18n("Maui Apps!")
        label1.font.bold: true
        label1.font.weight: Font.Bold
        label1.font.pointSize: Maui.Style.fontSizes.huge
        label2.text: i18n("Convergent set of apps for desktop and mobile computers.")
    }

    function itemWidth(index)
    {
        switch(index)
        {
        case 0: return 240;
        case 1:return 180;
        case 2: return 320;
        case 3: return 100
        }
    }

    function itemHeight(index)
    {
        switch(index)
        {
        case 0: return 360;
        case 1:return 360;
        case 2: return 100;
        case 3: return 100
        }
    }

    Flow
    {
        Layout.fillWidth: true
        Layout.maximumWidth: 500
        Layout.alignment: Qt.AlignCenter
        spacing: Maui.Style.space.medium

        Repeater
        {
            model: Maui.BaseModel
            {
                list: NX.Store
                {
                    id: _featureListBanner
                    category: _categoriesList.baseCategory()
                    pageSize: 4
                    sort: NX.Store.NEWEST
                    tags: ["mauikit"]
                }
            }

            delegate: Kirigami.ShadowedRectangle
            {
               width: itemWidth(index)
               height: itemHeight(index)

                color: Qt.darker(Maui.Theme.backgroundColor)

                corners
                {
                    topLeftRadius: radius
                    topRightRadius: radius
                    bottomLeftRadius: radius
                    bottomRightRadius: radius
                }

                shadow.xOffset: 0
                shadow.yOffset: 0
                shadow.color: Qt.rgba(0, 0, 0, 0.3)
                shadow.size: 8

                Item
                {
                    anchors.fill: parent
                    opacity: 0.3

                    Item
                    {
                        id: _bannerImage
                        anchors.fill: parent
                        clip: true
                        Image
                        {
                            anchors.centerIn: parent
                            width: parent.width *3
                            height: parent.height * 3
                            sourceSize.height: 64
                            sourceSize.width: 64
                            source: model.preview
                            fillMode: Image.PreserveAspectCrop
                            antialiasing: false
                            smooth: false
                            asynchronous: true
                            rotation: 150
                        }
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

//                Maui.ListItemTemplate
//                {
//                    //                            anchors.fill: parent
//                    anchors.centerIn: parent
//                    width: parent.width
//                    //                            anchors.margins: Maui.Style.space.huge

//                    label1.font.pointSize: Maui.Style.fontSizes.enormous * 2
//                    label1.font.bold: true
//                    label1.font.weight: Font.Bold
//                    label1.verticalAlignment: Qt.AlignVCenter

//                    label1.text: model.name
//                    label2.text:  model.typename + " - " + model.personid
//                    //                            label2.wrapMode: Text.WordWrap
//                    //                            label2.verticalAlignment: Qt.AlignTop

//                    iconVisible: isWide
//                    imageSizeHint: Maui.Style.iconSizes.huge * 2
//                    headerSizeHint: imageSizeHint * 1.5
//                    rightLabels.visible: isWide
//                    imageSource: model.preview

//                    fillMode: Image.PreserveAspectFit

//                }

            }
        }
    }
}

