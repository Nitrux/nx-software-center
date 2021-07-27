import QtQuick 2.14
import QtQuick.Controls 2.14

import org.kde.kirigami 2.14 as Kirigami
import org.mauikit.controls 1.3 as Maui

Maui.GalleryRollItem
{
    id: control
    isCurrentItem: GridView.isCurrentItem
    cache: true

    label1.font.bold: true
    label1.font.weight: Font.Bold
    label1.font.pointSize: Maui.Style.fontSizes.big
//    template.labelSizeHint: 32

    cb : function(model)
    {
        return model.pic
    }

    background: Kirigami.ShadowedRectangle
    {
        color: Qt.lighter(Kirigami.Theme.backgroundColor)
        property int radius : Maui.Style.radiusV

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
        shadow.size: 10
    }
 }
