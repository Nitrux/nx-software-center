import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.14 as Kirigami
import org.mauikit.controls 1.2 as Maui
import QtGraphicalEffects 1.0

Maui.ListBrowserDelegate
{
    id: control

    implicitWidth: Math.max(350, template.implicitWidth)

    label1.font.pointSize: Maui.Style.fontSizes.huge
    label1.font.weight: Font.Bold
    label1.font.bold: true
    label2.font.pointSize: Maui.Style.fontSizes.big
    label3.font.pointSize: Maui.Style.fontSizes.huge
    label3.font.weight: Font.Bold
    label3.font.bold: true

    rightLabels.visible: true

    background: Rectangle
    {
        radius: Maui.Style.radiusV
        color: Qt.tint(control.Kirigami.Theme.textColor, Qt.rgba(control.Kirigami.Theme.backgroundColor.r, control.Kirigami.Theme.backgroundColor.g, control.Kirigami.Theme.backgroundColor.b, 0.9))

        Rectangle
        {
            id: _iconRec
            opacity: 0.7
            anchors.fill: parent
            color: Kirigami.Theme.backgroundColor
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

            Rectangle
            {
                anchors.fill: parent
                opacity: 0.5
                color: Qt.tint(control.Kirigami.Theme.textColor, Qt.rgba(control.Kirigami.Theme.backgroundColor.r, control.Kirigami.Theme.backgroundColor.g, control.Kirigami.Theme.backgroundColor.b, 0.9))
            }
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
