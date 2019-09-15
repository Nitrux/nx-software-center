import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.kde.mauikit 1.0 as Maui

Maui.Page
{
    id: control

    headBar.middleContent: Maui.TextField
    {
        Layout.fillWidth: true
        placeholderText: qsTr("Search for apps, packages, etc...")

    }

    Maui.Holder
    {
        visible: true

    }
}
