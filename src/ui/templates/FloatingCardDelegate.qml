import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.14 as Kirigami
import org.kde.mauikit 1.2 as Maui
import QtGraphicalEffects 1.0

Maui.ItemDelegate
{
    id: control

    property alias label1 : _template.label1
    property alias label2 : _template.label2
    property alias label3 : _template.label3
    property alias iconSource : _icon.source
    property alias iconSizeHint: _icon.height

    implicitWidth: Math.max(350, _layout.implicitWidth)

    background: Rectangle
    {
        radius: Maui.Style.radiusV
        color: Qt.tint(control.Kirigami.Theme.textColor, Qt.rgba(control.Kirigami.Theme.backgroundColor.r, control.Kirigami.Theme.backgroundColor.g, control.Kirigami.Theme.backgroundColor.b, 0.9))

        border.color: control.isCurrentItem || control.hovered ? Kirigami.Theme.highlightColor : "transparent"

    }

    RowLayout
    {
        id: _layout
        anchors.fill: parent

        Item
        {
            id: _iconItem
            Layout.fillHeight: true
            implicitWidth: height
            clip: true
            Rectangle
            {
                id: _iconRec
                anchors.fill: parent
                color: Kirigami.Theme.backgroundColor

                FastBlur
                {
                    id: fastBlur
                    height: parent.height * 2
                    width: parent.width * 2
                    anchors.centerIn: parent
                    source: _icon
                    radius: 64
                    transparentBorder: false
                    cached: true
                }

                Rectangle
                {
                    anchors.fill: parent
                    opacity: 0.5
                    color: Qt.tint(control.Kirigami.Theme.textColor, Qt.rgba(control.Kirigami.Theme.backgroundColor.r, control.Kirigami.Theme.backgroundColor.g, control.Kirigami.Theme.backgroundColor.b, 0.9))
                }

                Kirigami.Icon
                {
                    id: _icon
                    anchors.centerIn: parent
                    width: height
                }
            }
        }


        Maui.ListItemTemplate
        {
            id: _template
            implicitWidth: leftLabels.implicitWidth
            Layout.fillWidth: true
            Layout.fillHeight: true
            label1.font.pointSize: Maui.Style.fontSizes.huge
            label1.font.weight: Font.Bold
            label1.font.bold: true
            label2.font.pointSize: Maui.Style.fontSizes.big
            label3.font.pointSize: Maui.Style.fontSizes.huge
            label3.font.weight: Font.Bold
            label3.font.bold: true

            rightLabels.visible: true
        }
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
                radius: Maui.Style.radiusV
            }
        }
    }

}
