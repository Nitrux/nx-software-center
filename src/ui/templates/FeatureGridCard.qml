import QtQuick 2.14
import QtQuick.Controls 2.14

import org.kde.kirigami 2.14 as Kirigami
import org.mauikit.controls 1.3 as Maui

Maui.GalleryRollItem
{
    id: control
    isCurrentItem: GridView.isCurrentItem
    cache: true

//    template.labelSizeHint: 32

    cb : function(model)
    {
        return model.pic
    }
 }
