import QtQuick 2.10
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

    Connections
    {
        target: _categoriesSidebar
        onItemClicked:
        {
            if(control.depth > 1)
                control.pop()
        }
    }

    Component
    {
        id: _appPageComponent

        AppPage
        {
            id: _appPage
            onExit: control.pop()

            onPackageClicked:
            {
                console.log("trying to append package app ", app.id)
                _progressView.manager.appendPackage(app, index, NX.Package.DOWNLOAD)
            }

            buttonActions: [

                Button
                {
                    text: qsTr("Download")
                    visible: !app.isInstalled
                    Kirigami.Theme.textColor: "#37474F"
                    Kirigami.Theme.backgroundColor: Qt.rgba("#546E7A".r, "#546E7A".g, "#546E7A".b, 0.2)
                    onClicked: {
                        _appPage.scrollTo(AppPage.Sections.Packages)
                    }
                },

                Button
                {
                    text: qsTr("Run")
                    visible: app.isInstalled
                    onClicked: app.launchApp()
                    Kirigami.Theme.textColor: "#37474F"
                    Kirigami.Theme.backgroundColor: Qt.rgba("#546E7A".r, "#546E7A".g, "#546E7A".b, 0.2)

                },

                Button
                {
                    text: qsTr("Update")
                    visible: app.isInstalled && app.isUpdatable
                    onClicked: app.updateApp()
                    Kirigami.Theme.textColor: Kirigami.Theme.positiveTextColor
                    Kirigami.Theme.backgroundColor: Qt.rgba(Kirigami.Theme.positiveTextColor.r, Kirigami.Theme.positiveTextColor.g, Kirigami.Theme.positiveTextColor.b, 0.2)

                },

                Button
                {
                    text: qsTr("Remove")
                    visible: app.isInstalled
                    onClicked: app.removeApp()
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
            Maui.TextField
            {
                Layout.fillWidth: true
                placeholderText: qsTr("Search package ...")
                onAccepted: _storeList.nameFilter = text
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

        ListView
        {
            id: _listView
            anchors.fill: parent

            Layout.fillWidth: true
            Layout.fillHeight: true
            orientation: ListView.Vertical
            spacing: Maui.Style.space.medium
            ScrollBar.vertical: ScrollBar {
              active: true
              anchors {
                right: _listView.right
                top: _listView.top
                bottom : _listView.bottom
                rightMargin: -18
              }
            }
            onAtYEndChanged:
            {
                if(_listView.atYEnd)
                {
                    const prevPos = _listView.contentY
                    _storeList.page ++

                }
            }

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
                    radius: Maui.Style.radiusV * 2

                    Rectangle
                    {
                        id: _banner
                        anchors.fill: parent

                        Image
                        {
                            id: _bannerImage
                            anchors.fill : parent

                            sourceSize.height: height /50
                            sourceSize.width: width /50
                            source: _featuredListview.currentItem.imageSource
                            fillMode: Image.PreserveAspectCrop
                            antialiasing: false
                            smooth: false
                            asynchronous: true

                            layer.enabled: true
                            layer.effect: OpacityMask
                            {
                                maskSource: Item
                                {
                                    width: _bannerImage.width
                                    height: _bannerImage.height

                                    Rectangle
                                    {
                                        anchors.centerIn: parent
                                        width: _bannerImage.width
                                        height: _bannerImage.height
                                        radius: _featuredSection.radius
                                    }
                                }
                            }

                        }
                    }

                    FastBlur
                    {
                        id: fastBlur
                        anchors.fill: _banner
                        source: _banner
                        radius: 90
                        transparentBorder: false
                        cached: true

                        layer.enabled: true
                        layer.effect: OpacityMask
                        {
                            maskSource: Item
                            {
                                width: fastBlur.width
                                height: fastBlur.height

                                Rectangle
                                {
                                    anchors.centerIn: parent
                                    width: fastBlur.width
                                    height: fastBlur.height
                                    radius: _featuredSection.radius
                                }
                            }
                        }
                    }

                    Rectangle
                    {
                        anchors.fill: parent
                        color: Kirigami.Theme.backgroundColor
                        radius: _featuredSection.radius
                        opacity: 0.7
                        border.color: Qt.tint(Kirigami.Theme.textColor, Qt.rgba(Kirigami.Theme.backgroundColor.r, Kirigami.Theme.backgroundColor.g, Kirigami.Theme.backgroundColor.b, 0.7))
                    }



                    ListView
                    {
                        id: _featuredListview
                        anchors.fill: parent
                        orientation: ListView.Horizontal
                        snapMode: ListView.SnapOneItem
                        clip: true
                        highlightMoveDuration: 1000
                        highlightMoveVelocity: -1

                        onMovementEnded: currentIndex = indexAt(contentX, contentY)

                        MouseArea
                        {
                            id: _featureMouseArea
                            anchors.fill: parent
                            hoverEnabled: true
                            propagateComposedEvents: true
                        }
                        Timer
                        {
                            id: _featuredListviewTimer
                            interval: 7000
                            repeat: true
                            running: !_featureMouseArea.containsPress || !_featureMouseArea.containsMouse
                            onTriggered: _featuredListview.cycleSlideForward()
                        }

                        Row
                        {
                            spacing: Maui.Style.space.medium
                            anchors.horizontalCenter: parent.horizontalCenter
                            z: 999
                            anchors.bottom: parent.bottom
                            anchors.margins: Maui.Style.space.big

                            Repeater
                            {
                                model: _featureList.count

                                Rectangle
                                {
                                    width: Maui.Style.iconSizes.tiny
                                    height: width
                                    radius: width

                                    color: "grey"
                                    opacity: index === _featuredListview.currentIndex ? 1 : 0.5
                                }
                            }

                        }

                        Rectangle
                        {
                            color: "#333"
                            border.color: Qt.darker(color)
                            height: Maui.Style.iconSizes.medium + Kirigami.Units.smallSpacing
                            width: height
                            radius: Maui.Style.radiusV
                            anchors.left: parent.left
                            anchors.margins: Maui.Style.space.medium
                            anchors.verticalCenter: parent.verticalCenter

                            ToolButton
                            {
                                anchors.centerIn: parent
                                height: Maui.Style.iconSizes.small
                                width: height
                                icon.width: height
                                icon.color: "white"
                                icon.name: "go-previous"
                                onClicked: _featuredListview.cycleSlideBackward()
                            }
                        }

                        Rectangle
                        {
                            anchors.right: parent.right
                            anchors.margins: Maui.Style.space.medium
                            anchors.verticalCenter: parent.verticalCenter
                            color: "#333"
                            border.color: Qt.darker(color)
                            height: Maui.Style.iconSizes.medium + Kirigami.Units.smallSpacing
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
                                onClicked: _featuredListview.cycleSlideForward()
                            }
                        }


                        model: Maui.BaseModel
                        {
                            list: NX.Store
                            {
                                id: _featureList
                                category: _categoriesSidebar.list.featureCategory()
                                pageSize: 5
                                sort: NX.Store.MOST_DOWNLOADED
                            }
                        }

                        delegate: Item
                        {

                            property url imageSource: model.preview
                            height: _featuredListview.height
                            width: _featuredListview.width

                            MouseArea {
                                id: _featuredDelegateClickable
                                anchors.fill: parent
                            }

                            RowLayout
                            {
                                spacing: 0
                                anchors.fill: parent

                                Item
                                {
                                    Layout.fillHeight: true
                                    Layout.preferredWidth: Maui.Style.iconSizes.huge * 1.5
                                    Layout.margins: 1
                                    Layout.alignment: Qt.AlignCenter

                                    Image
                                    {
                                        height: Maui.Style.iconSizes.huge
                                        width: height
                                        sourceSize.height: height
                                        sourceSize.width: width
                                        anchors.centerIn: parent
                                        fillMode: Image.PreserveAspectFit
                                        source: imageSource
                                    }
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
                                            text: model.personid
                                            visible: text.length
                                            Layout.fillWidth: true
                                            font.weight: Font.Light
                                            elide: Text.ElideMiddle
                                            color: Kirigami.Theme.textColor
                                        }

                                        Label
                                        {
                                            text: model.version
                                            visible: text.length
                                            Layout.fillWidth: true
                                            font.weight: Font.Light
                                            elide: Text.ElideMiddle
                                            color: Kirigami.Theme.textColor
                                        }

                                        Label
                                        {
                                            text: model.description
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

                            Connections
                            {
                                target: _featuredDelegateClickable
                                onClicked:
                                {
                                    control.push(_appPageComponent)
                                    _storeList.setApp(_featuredListview.model.get(_featuredListview.currentIndex).id)
                                    control.currentItem.data = _storeList.app
                                }
                            }
                        }

                        function cycleSlideForward() {
                          _featuredListviewTimer.restart();

                          if (_featuredListview.currentIndex === _featuredListview.count - 1) {
                            _featuredListview.currentIndex = 0;
                          } else {
                            _featuredListview.incrementCurrentIndex();
                          }
                        }

                        function cycleSlideBackward() {
                          _featuredListviewTimer.restart();

                          if (_featuredListview.currentIndex === 0) {
                            _featuredListview.currentIndex = _featuredListview.count - 1;
                          } else {
                            _featuredListview.decrementCurrentIndex();
                          }
                        }
                    }
                }
            }

            model: Maui.BaseModel
            {
                id: _appsModel
                list: NX.Store
                {
                    id: _storeList
                    category: _categoriesSidebar.currentCategory
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

//                quickActions: [
//                    Action
//                    {
//                        icon.name: "document-share"
//                    },

//                    Action
//                    {
//                        icon.name: "media-playback-start"
//                    },

//                    Action
//                    {
//                        icon.name: "entry-delete"
//                    },

//                    Action
//                    {
//                        icon.name: "download"
//                    }

//                ]

                Connections
                {
                    target: _delegate
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
    }
}
