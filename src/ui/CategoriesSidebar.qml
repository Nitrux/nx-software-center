import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.kde.mauikit 1.0 as Maui

Maui.GlobalDrawer
{
    property bool collapsed : !root.isWide
    width: currentView !== views.system ? (collapsed ? _sidebar.iconSize * 2 : Math.min(Kirigami.Units.gridUnit * 11, root.width)) : 0
    modal: false

    contentItem:  Maui.SideBar
    {
        id: _sidebar
        anchors.fill: parent
        iconSize: Kirigami.Units.iconSizes.smallMedium
        showLabels: !collapsed

        section.property: collapsed ? "" : "category"
        section.criteria: ViewSection.FullString
        section.delegate: Maui.LabelDelegate
        {
            id: delegate
            label: section
            labelTxt.font.pointSize: fontSizes.big

            isSection: true
            boldLabel: true
            height: Maui.Style.toolBarHeightAlt
        }
        ScrollBar.vertical.policy: collapsed ? ScrollBar.AlwaysOff : ScrollBar.AlwaysOn

        model: ListModel
        {
            ListElement{ category: "Applications"; label: "All"; icon: "appimage-store";}
            ListElement{ category: "Applications"; label: "Education"; icon: "applications-education";}
            ListElement{ category: "Applications"; label: "Development"; icon: "applications-development";}
            ListElement{ category: "Applications"; label: "Graphics"; icon: "applications-graphics"; count: 130000}
            ListElement{ category: "Applications"; label: "Internet"; icon: "applications-internet"}
            ListElement{ category: "Applications"; label: "Games"; icon: "applications-games"; count: 7}
            ListElement{ category: "Applications"; label: "Multimedia"; icon: "applications-multimedia"}
            ListElement{ category: "Applications"; label: "Office"; icon: "applications-office"}
            ListElement{ category: "Customization"; label: "Plasma Themes"; icon: "start-here"}
            ListElement{ category: "Customization"; label: "Plasma Color"; icon: "start-here"}
            ListElement{ category: "Customization"; label: "GTK Themes"; icon: "start-here"}
            ListElement{ category: "Customization"; label: "Cursors"; icon: "start-here"}
            ListElement{ category: "Customization"; label: "Wallpapers"; icon: "start-here"}
            ListElement{ category: "Customization"; label: "Icons"; icon: "start-here"}
        }

        onItemClicked:
        {
            _swipeView.currentItem.categoryUri = _sidebar.model.get(index).label
        }
    }

}
