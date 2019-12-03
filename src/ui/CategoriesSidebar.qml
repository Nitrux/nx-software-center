import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.kde.mauikit 1.0 as Maui

import NXModels 1.0 as NX

Maui.SideBar
{
    id: control

    property alias currentCategory : _categoriesList.currentCategory

    collapsible: true
    collapsed: !root.isWide
    preferredWidth: count > 0 ? Math.min(Kirigami.Units.gridUnit * 11, root.width) : 0

    iconSize : Maui.Style.iconSizes.medium
    section.property: !showLabels ? "" : "category"
    section.criteria: ViewSection.FullString
    section.delegate: Maui.LabelDelegate
    {
        id: delegate
        label: section
        labelTxt.font.pointSize: Maui.Style.fontSizes.big

        isSection: true
        labelTxt.font.bold: true
        height: Maui.Style.toolBarHeightAlt
        width: parent.width
    }
    //        ScrollBar.vertical.policy: collapsed ? ScrollBar.AlwaysOff : ScrollBar.AlwaysOn

    model: Maui.BaseModel
    {
        id: _appsModel
        list: NX.Categories
        {
            id: _categoriesList
        }
    }

    delegate: Maui.ListDelegate
    {
        id: itemDelegate
        iconSize: control.iconSize
        labelVisible: control.showLabels
        label: model.title
        count: model.count > 0 ? model.count : ""
        iconName: model.icon +  (Qt.platform.os == "android" ? ("-sidebar") : "")
        leftPadding:  Maui.Style.space.tiny
        rightPadding:  Maui.Style.space.tiny

        Connections
        {
            target: itemDelegate
            onClicked:
            {
                control.currentIndex = index
                control.itemClicked(index)
            }

            onRightClicked:
            {
                control.currentIndex = index
                control.itemRightClicked(index)
            }

            onPressAndHold:
            {
                control.currentIndex = index
                control.itemRightClicked(index)
            }
        }
    }

    onItemClicked:
    {
       _categoriesList.setCurrentCategory(control.model.get(index).id)
    }
}


