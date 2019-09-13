import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.kde.mauikit 1.0 as Maui

Maui.GlobalDrawer
{
    property bool collapsed : !root.isWide
    width: collapsed ? _sidebar.iconSize * 2 : Math.min(Kirigami.Units.gridUnit * 11, root.width)
    modal: false

    contentItem:  Maui.SideBar
    {
        id: _sidebar
        anchors.fill: parent
        //        orientation: ListView.Vertical
        iconSize: Kirigami.Units.iconSizes.smallMedium
        showLabels: !collapsed

        model: ListModel
        {
            ListElement{ category: "Applications"; label: "All"; icon: "appimage-store"; count: 72000}
            ListElement{ category: "Applications"; label: "Education"; icon: "applications-education"; count: 650}
            ListElement{ category: "Applications"; label: "Development"; icon: "applications-development"; count: 300}
            ListElement{ category: "Applications"; label: "Graphics"; icon: "applications-graphics"; count: 13}
            ListElement{ category: "Applications"; label: "Internet"; icon: "applications-internet"}
            ListElement{ category: "Applications"; label: "Games"; icon: "applications-games"; count: 7}
            ListElement{ category: "Applications"; label: "Multimedia"; icon: "applications-multimedia"}
            ListElement{ category: "Applications"; label: "Office"; icon: "applications-office"}
        }

        //        delegate: Maui.ListDelegate
        //        {

        //        }
    }

}
