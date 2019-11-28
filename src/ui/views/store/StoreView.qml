import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.kde.mauikit 1.0 as Maui
import QtGraphicalEffects 1.0

import "../../templates"
import NXModels 1.0 as NX

StackView
{
    id: control
    property alias category : _storeList.category

    Component
    {
        id: _appPageComponent

        AppPage
        {
            onExit: control.pop()
            buttonActions: [

                Button
                {
                    text: qsTr("Download")
                    Kirigami.Theme.textColor: "#37474F"
                    Kirigami.Theme.backgroundColor: Qt.rgba("#546E7A".r, "#546E7A".g, "#546E7A".b, 0.2)

                },

                Button
                {
                    text: qsTr("Run")
                    Kirigami.Theme.textColor: "#37474F"
                    Kirigami.Theme.backgroundColor: Qt.rgba("#546E7A".r, "#546E7A".g, "#546E7A".b, 0.2)

                },

                Button
                {
                    text: qsTr("Update")
                    Kirigami.Theme.textColor: Kirigami.Theme.positiveTextColor
                    Kirigami.Theme.backgroundColor: Qt.rgba(Kirigami.Theme.positiveTextColor.r, Kirigami.Theme.positiveTextColor.g, Kirigami.Theme.positiveTextColor.b, 0.2)

                },

                Button
                {
                    text: qsTr("Remove")
                    Kirigami.Theme.textColor: Kirigami.Theme.negativeTextColor
                    Kirigami.Theme.backgroundColor: Qt.rgba(Kirigami.Theme.negativeTextColor.r, Kirigami.Theme.negativeTextColor.g, Kirigami.Theme.negativeTextColor.b, 0.2)

                }
            ]
        }
    }

    initialItem: Maui.Page
    {
        padding: control.depth === 2 ? 0 : Maui.Style.space.big

        headBar.middleContent: [
            Maui.PathBar
            {
                Layout.fillWidth: true              
                url: "Development/IDE/Sub"
                onHomeClicked: url = "Development/"
//                onPlaceClicked: browser.openFolder(path)

                Maui.TextField
                {
                    visible: false
                }
            }
        ]

        headBar.rightContent: ToolButton
        {
            icon.name: "view-sort"
        }


        ListView
        {
            id: _listView
            anchors.fill: parent

            Layout.fillWidth: true
            Layout.fillHeight: true
            orientation: ListView.Vertical
            spacing: Maui.Style.space.medium

            //            headerPositioning: ListView.PullBackFooter
            header: Item
            {
                height: 250
                width: parent.width

                Rectangle
                {
                    id: _featuredSection
                    height: 200
                    width: parent.width
                    anchors.centerIn: parent
                    Kirigami.Theme.colorSet: Kirigami.Theme.Button
                    Kirigami.Theme.inherit: false
                    color: Kirigami.Theme.backgroundColor
                    border.color: Qt.tint(Kirigami.Theme.textColor, Qt.rgba(Kirigami.Theme.backgroundColor.r, Kirigami.Theme.backgroundColor.g, Kirigami.Theme.backgroundColor.b, 0.7))
                    radius: Maui.Style.radiusV


                    ListView
                    {
                        id: _featuredListview
                        anchors.fill: parent
                        orientation: ListView.Horizontal
                        snapMode: ListView.SnapOneItem
                        clip: true

                        onMovementEnded: currentIndex = indexAt(contentX, contentY)

                        Row
                        {
                            spacing: Maui.Style.space.big
                            anchors.horizontalCenter: parent.horizontalCenter
                            z: 999
                            anchors.bottom: parent.bottom
                            anchors.margins: Maui.Style.space.big

                            Rectangle
                            {
                                visible: _featuredListview.count > 0 && _featuredListview.currentIndex > 0
                                color: _countBadge.Kirigami.Theme.backgroundColor
                                border.color: Qt.darker(color)
                                height: Maui.Style.iconSizes.small + Kirigami.Units.smallSpacing
                                width: height
                                radius: Maui.Style.radiusV
                                anchors.verticalCenter: parent.verticalCenter
                                ToolButton
                                {
                                    anchors.centerIn: parent
                                    height: Maui.Style.iconSizes.small
                                    width: height
                                    icon.width: height
                                    icon.color: "white"
                                    icon.name: "go-previous"
                                    onClicked: _featuredListview.decrementCurrentIndex()
                                }
                            }

                            Maui.Badge
                            {
                                id: _countBadge
                                text: _featuredListview.currentIndex + 1
                                anchors.verticalCenter: parent.verticalCenter
                            }

                            Rectangle
                            {
                                visible: _featuredListview.count > 0 && _featuredListview.currentIndex < _featuredListview.count - 1

                                anchors.verticalCenter: parent.verticalCenter
                                color: _countBadge.Kirigami.Theme.backgroundColor
                                border.color: Qt.darker(color)
                                height: Maui.Style.iconSizes.small + Kirigami.Units.smallSpacing
                                width: height
                                radius: Maui.Style.radiusV
                                ToolButton
                                {
                                    anchors.centerIn: parent
                                    height: Maui.Style.iconSizes.small
                                    width: height
                                    icon.color: "white"
                                    icon.width: height
                                    icon.name: "go-next"
                                    onClicked: _featuredListview.incrementCurrentIndex()
                                }
                            }
                        }

                        model: ListModel
                        {
                            ListElement {name:"Index"; author:"Camilo Higuita"; organization:"Maui"; version: "1.0.0";  iconName: "index"; bannerImg: "qrc:/tests/banner_index.png";  count: 72000; itemInfo: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper."}
                            ListElement {name:"Vvave"; version: "1.0.0";  iconName: "vvave"; bannerImg: "qrc:/tests/banner_index.png";  count: 72000; itemInfo: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper."}
                        }

                        delegate: RowLayout
                        {
                            spacing: 0
                            height: _featuredListview.height
                            width: _featuredListview.width

                            Item
                            {
                                Layout.fillHeight: true
                                Layout.preferredWidth: parent.height
                                Layout.margins: 1
                                Layout.alignment: Qt.AlignCenter

                                Image
                                {
                                    id: _bannerImage
                                    height: parent.height
                                    width: parent.width
                                    sourceSize.height: height
                                    sourceSize.width: width
                                    source: model.bannerImg
                                    fillMode: Image.PreserveAspectCrop
                                    antialiasing: true
                                    smooth: true
                                    asynchronous: true

                                }

                                FastBlur
                                {
                                    id: fastBlur
                                    anchors.fill: parent
                                    source: _bannerImage
                                    radius: 50
                                    transparentBorder: false
                                    cached: true
                                }

                                Rectangle
                                {
                                    anchors.fill: parent
                                    color: Kirigami.Theme.viewBackgroundColor
                                    opacity: 0.7
                                }

                                Kirigami.Icon
                                {
                                    height: Maui.Style.iconSizes.huge
                                    width: height
                                    anchors.centerIn: parent
                                    source: model.iconName
                                }
                            }

                            Kirigami.Separator
                            {
                                Layout.fillHeight: true
                                Layout.preferredWidth: 1
                            }

                            Item
                            {
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                Layout.margins: Maui.Style.space.huge

                                ColumnLayout
                                {
                                    anchors.fill: parent
                                    spacing: 0
                                    Label
                                    {
                                        text: model.name
                                        visible: text.length
                                        Layout.fillWidth: true
                                        font.bold: true
                                        font.weight: Font.Bold
                                        elide: Text.ElideMiddle
                                        color: Kirigami.Theme.textColor
                                    }

                                    Label
                                    {
                                        text: model.author
                                        visible: text.length
                                        Layout.fillWidth: true
                                        font.weight: Font.Light
                                        elide: Text.ElideMiddle
                                        color: Kirigami.Theme.textColor
                                    }

                                    Label
                                    {
                                        text: model.organization
                                        visible: text.length
                                        Layout.fillWidth: true
                                        font.weight: Font.Light
                                        elide: Text.ElideMiddle
                                        color: Kirigami.Theme.textColor
                                    }

                                    Label
                                    {
                                        text: model.itemInfo
                                        visible: text.length
                                        Layout.topMargin: Maui.Style.space.big
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        font.pointSize: Maui.Style.fontSizes.small
                                        font.weight: Font.Light
                                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                                        elide: Text.ElideRight
                                        color: Kirigami.Theme.textColor
                                    }
                                }
                            }
                        }
                    }
                }
            }

            model:  Maui.BaseModel
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
                height: 100
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                label1.text: model.name
                label2.text: model.description
                label3.text: model.totaldownloads + qsTr(" Downloads")
                label4.text: model.score + qsTr(" Points")
                imageSource: model.smallpic
                iconSizeHint: height * 0.7

                quickActions: [
                    Action
                    {
                        icon.name: "document-share"
                    },

                    Action
                    {
                        icon.name: "media-playback-start"
                    },

                    Action
                    {
                        icon.name: "entry-delete"
                    }
                ]

                Connections
                {
                    target: _delegate
                    onClicked:
                    {
                        _listView.currentIndex = index
                        control.push(_appPageComponent)
                        control.currentItem.appInfo = _storeList.getApp(_listView.model.get(_listView.currentIndex).id)
                    }
                }

            }
        }



    }
}
