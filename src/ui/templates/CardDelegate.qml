import QtQuick 2.9
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import org.mauikit.controls 1.0 as Maui
import org.kde.kirigami 2.2 as Kirigami

Maui.ItemDelegate
{
    id: control

    Kirigami.Theme.colorSet: Kirigami.Theme.Button
    Kirigami.Theme.inherit: false

    property alias quickButtons : _template.content

    property alias label1 : _template.label1
    property alias label2 : _template.label2
    property alias label3 : _badge1
    property alias label4 : _badge2

    property alias iconImg : _icon
    property int radius : Maui.Style.radiusV


  Maui.ListItemTemplate
  {
      id: _template
      anchors.fill: parent
  }
}
