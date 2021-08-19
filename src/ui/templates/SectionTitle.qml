import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3

import org.kde.kirigami 2.14 as Kirigami
import org.mauikit.controls 1.2 as Maui

Maui.ListItemTemplate
{
    Layout.fillWidth: true
    Layout.margins: Maui.Style.space.medium
    implicitHeight: leftLabels.implicitHeight
    label1.font.pointSize: Maui.Style.fontSizes.huge
    label1.font.bold: true
    label1.font.weight: Font.Bold
    label2.font.pointSize: Maui.Style.fontSizes.big
}

