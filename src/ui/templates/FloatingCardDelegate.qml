import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3

import org.mauikit.controls 1.3 as Maui
import QtGraphicalEffects 1.0

Maui.ListBrowserDelegate
{
    id: control
    implicitHeight: 80

    implicitWidth: Math.max(250, template.implicitWidth)
    iconSizeHint: 48
    template.imageSizeHint: 48
    iconSource: "package"
    label1.font.pointSize: Maui.Style.fontSizes.huge
    label1.font.weight: Font.Bold
    label1.font.bold: true
    label2.font.pointSize: Maui.Style.fontSizes.big
    label3.font.pointSize: Maui.Style.fontSizes.huge
    label3.font.weight: Font.Bold
    label3.font.bold: true

    rightLabels.visible: true

    background: Maui.ShadowedRectangle
    {
        color: control.hovered || control.highlighted ? Maui.Theme.highlightColor : Qt.tint(control.Maui.Theme.textColor, Qt.rgba(control.Maui.Theme.backgroundColor.r, control.Maui.Theme.backgroundColor.g, control.Maui.Theme.backgroundColor.b, 0.9))

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
        shadow.size: 6

        Item
        {
            id: _iconRec
            opacity: 0.2
            anchors.fill: parent
            clip: true

            FastBlur
            {
                id: fastBlur
                height: parent.height * 2
                width: parent.width * 2
                anchors.centerIn: parent
                source: control.template.iconItem
                radius: 64
                transparentBorder: true
                cached: true
            }

            OpacityMask
            {
                source: mask
                maskSource: _iconRec
            }

            LinearGradient
            {
                id: mask
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position: 0.2; color: "transparent"}
                    GradientStop { position: 0.5; color: control.background.color}
                }

                start: Qt.point(0, 0)
                end: Qt.point(_iconRec.width, _iconRec.height)
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
    }
}
