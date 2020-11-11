import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.14 as Kirigami
import org.kde.mauikit 1.2 as Maui
import QtGraphicalEffects 1.0

Maui.Page
{
    id: control
    headBar.middleContent: [
        Maui.TextField
        {
            Layout.fillWidth: true
            placeholderText: qsTr("Search package ...")
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

    Kirigami.ScrollablePage
    {
        anchors.fill: parent

        ListView
        {
            id: _listView
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

            //            headerPositioning: ListView.PullBackFooter
            header: Item
            {
                height: 250
                width: parent.width * 0.9
                anchors.horizontalCenter: parent.horizontalCenter

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
//                                category: _categoriesSidebar.list.featureCategory()
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
//                    category: _categoriesSidebar.currentCategory
                }
            }

            delegate: Maui.SwipeBrowserDelegate
            {
                id: _delegate
                height: Math.min(template.leftLabels.implicitHeight + Maui.Style.space.huge, 100)
                width: ListView.view.width * 0.9
                anchors.horizontalCenter: parent.horizontalCenter
                label1.text: model.name
                label1.font.pointSize: Maui.Style.fontSizes.big
                label1.font.bold: true
                label1.font.weight: Font.Bold
                //                    label2.wrapMode: Text.WordWrap
                //                    label2.text: model.description.slice(0, Math.min(model.description.length, 100))
                label2.text: model.totaldownloads + qsTr(" Downloads") + "\n" + model.score + qsTr(" Points")
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
}
