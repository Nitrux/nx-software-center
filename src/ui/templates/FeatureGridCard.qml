import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.14 as Kirigami
import org.kde.mauikit 1.3 as Maui
import QtGraphicalEffects 1.0

Maui.GalleryRollItem
{
    id: control
    isCurrentItem: GridView.isCurrentItem
    cache: true


    cb : function(model)
    {
        return model.pic
    }
 }
