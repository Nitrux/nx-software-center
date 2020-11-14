import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.14 as Kirigami
import org.kde.mauikit 1.2 as Maui
import QtGraphicalEffects 1.0

Maui.ItemDelegate
{
    id: control

    Kirigami.Theme.inherit: false
    Kirigami.Theme.backgroundColor: "#333"
    Kirigami.Theme.textColor: "#fafafa"

    property var images : []
    property alias label1 : _template.label1
    property alias label2 : _template.label2
    property alias label3 : _template.label3
    property alias label4 : _template.label4
    property alias iconSource : _template.iconSource
    property alias iconSizeHint: _template.iconSizeHint

    Component.onCompleted: _featuredTimer.start()

    Item
    {
        id: _cover
        anchors.fill: parent
        anchors.margins: isWide ? Maui.Style.space.big : Maui.Style.space.medium

        Rectangle
        {
            anchors.fill: parent
            color: Kirigami.Theme.backgroundColor
            radius: Maui.Style.radiusV

            HoverHandler
            {
                id: _hoverHandler
            }

            Timer
            {
                id: _featuredTimer
                interval: 6000
                repeat: true
                onTriggered: _featuredRoll.cycleSlideForward()
            }

            ListView
            {
                id: _featuredRoll
                anchors.fill: parent
                interactive: false
                orientation: Qt.Horizontal
                snapMode: ListView.SnapOneItem
                clip: true

                model: control.images

                function cycleSlideForward()
                {
                    _featuredTimer.restart()

                    if (_featuredRoll.currentIndex === _featuredRoll.count - 1)
                    {
                        _featuredRoll.currentIndex = 0
                    } else
                    {
                        _featuredRoll.incrementCurrentIndex()
                    }
                }

                function cycleSlideBackward()
                {
                    _featuredTimer.restart()

                    if (_featuredRoll.currentIndex === 0)
                    {
                        _featuredRoll.currentIndex = _featuredRoll.count - 1;
                    } else
                    {
                        _featuredRoll.decrementCurrentIndex();
                    }
                }

                delegate: Image
                {
//                    anchors.centerIn: parent
                    width: ListView.view.width
                    height: ListView.view.height * (_hoverHandler.hovered ? 1.2 : 1)
                    source: modelData.pic
                    fillMode: Image.PreserveAspectCrop
                    sourceSize.height: ListView.view.height
                    sourceSize.width : ListView.view.height

                    horizontalAlignment: Qt.AlignHCenter
                    verticalAlignment: Qt.AlignVCenter

                    Behavior on height
                    {
                        NumberAnimation
                        {
                            duration: Kirigami.Units.shortDuration
                            easing.type: Easing.InOutQuad
                        }
                    }
                }
            }

            LinearGradient
            {
                anchors.fill: parent
                start: Qt.point(0, 0)
                end: Qt.point(0, parent.height)
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "transparent" }
                    GradientStop { position: 0.9; color: control.Kirigami.Theme.backgroundColor }
                }
            }

            Maui.ListItemTemplate
            {
                id: _template
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: Maui.Style.space.medium
                implicitHeight: Math.max(100, leftLabels.implicitHeight)
                //            color: Kirigami.Theme.textColor
                label1.font.bold: true
                label1.font.weight: Font.Bold
                label1.font.pointSize: Maui.Style.fontSizes.enormous
                label2.font.pointSize: Maui.Style.fontSizes.big
                label3.font.pointSize: Maui.Style.fontSizes.enormous
                label4.font.pointSize: Maui.Style.fontSizes.small
                label3.font.bold: true
                label3.font.weight: Font.Bold

                label1.wrapMode: Text.WordWrap
                label1.elide: Text.ElideRight
                //            horizontalAlignment: Qt.AlignHCenter
            }

            layer.enabled: true
            layer.effect: OpacityMask
            {
                maskSource: Item
                {
                    width: _cover.width
                    height: _cover.height

                    Rectangle
                    {
                        anchors.fill: parent
                        radius: Maui.Style.radiusV
                    }
                }
            }
        }
    }
}
