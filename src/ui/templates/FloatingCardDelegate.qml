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

    background: Item {}

    implicitWidth: Math.max(350, _template.leftLabels.implicitWidth + height)

    Rectangle
    {
        id: _card
        anchors.fill: parent
        anchors.margins: Maui.Style.space.medium
        radius: Maui.Style.radiusV
        color: Qt.tint(control.Kirigami.Theme.textColor, Qt.rgba(control.Kirigami.Theme.backgroundColor.r, control.Kirigami.Theme.backgroundColor.g, control.Kirigami.Theme.backgroundColor.b, 0.9))

        Maui.ListItemTemplate
        {
            id: _template
            anchors.fill: parent
            anchors.leftMargin: _iconRec.width
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

    Rectangle
    {
        id: _iconRec
        width: height
        height: parent.height
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
            radius: Maui.Style.radiusV

        }

        Kirigami.Icon
        {
            id: _icon
            anchors.centerIn: parent
            width: height
        }

        layer.enabled: true
        layer.effect: OpacityMask
        {
            maskSource: Item
            {
                width: _iconRec.width
                height: _iconRec.height

                Rectangle
                {
                    anchors.fill: parent
                    radius: Maui.Style.radiusV
                }
            }
        }
    }

}
