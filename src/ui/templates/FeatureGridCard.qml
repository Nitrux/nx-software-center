import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3

import org.mauikit.controls 1.3 as Maui

Maui.ItemDelegate
{
    id: control
    property bool isCurrentItem: GridView.isCurrentItem

    property alias header : _header
    property alias template : _template
    property alias label1 : _template.label1
    property alias label2 : _template.label2
    property alias label3 : _template.label3
    property alias label4 : _template.label4
    property alias images : _header.images
    property alias imageSource: _template.imageSource

    ColumnLayout
    {
        anchors.fill: parent
        spacing: Maui.Style.space.medium

        Maui.GalleryRollTemplate
        {
            id: _header
            Layout.fillHeight: true
            Layout.fillWidth: true
            cache: true
            orientation: Qt.Vertical
            radius: control.radius

            cb : function(model)
            {
                return model.pic
            }
        }

        Maui.ListItemTemplate
        {
            id: _template
            //        Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.leftMargin: Maui.Style.space.small
            Layout.rightMargin: Maui.Style.space.small
            Layout.preferredHeight: 52
            isCurrentItem: control.isCurrentItem
            iconSource: "package"
            imageSizeHint: 40
            iconSizeHint: 22
            fillMode: Image.PreserveAspectFit
        }
    }
}
