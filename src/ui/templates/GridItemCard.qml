import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.14 as Kirigami
import org.kde.mauikit 1.2 as Maui
import QtGraphicalEffects 1.0

Maui.ItemDelegate
{
    id: control

    property alias label1 : _label1
    property alias iconSource : _icon.source
    property alias iconSizeHint: _icon.height

    ColumnLayout
    {
        anchors.fill: parent
        spacing: Maui.Style.space.medium
        anchors.margins: Maui.Style.space.medium

        Item
        {
            Layout.fillWidth: true
            Layout.fillHeight: true

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
                isMask: true
                color: Kirigami.Theme.textColor
            }
        }

        Label
        {
            id: _label1
            Layout.fillWidth: true
            color: Kirigami.Theme.textColor
            wrapMode: Text.WordWrap
            elide: Text.ElideRight
            horizontalAlignment: Qt.AlignHCenter
        }
    }
}
