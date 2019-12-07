import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.kde.mauikit 1.0 as Maui
import NXModels 1.0 as NX
import "../../templates"

StackView
{
    id: control

    property bool isActive: true
    property alias manager : _progressManager

    NX.ProgressManager
    {
        id: _progressManager
        onWarning: notify("package-x-generic", "Oops", message)
    }

    Component
    {
        id: _appPageComponent

        AppPage
        {
            onExit: control.pop()
            buttonActions: [
                Button
                {
                    text: qsTr("Run")
                    Kirigami.Theme.textColor: "#37474F"
                    Kirigami.Theme.backgroundColor: Qt.rgba("#546E7A".r, "#546E7A".g, "#546E7A".b, 0.2)

                },

                Button
                {
                    text: qsTr("Update")
                    Kirigami.Theme.textColor: Kirigami.Theme.positiveTextColor
                    Kirigami.Theme.backgroundColor: Qt.rgba(Kirigami.Theme.positiveTextColor.r, Kirigami.Theme.positiveTextColor.g, Kirigami.Theme.positiveTextColor.b, 0.2)

                },

                Button
                {
                    text: qsTr("Remove")
                    Kirigami.Theme.textColor: Kirigami.Theme.negativeTextColor
                    Kirigami.Theme.backgroundColor: Qt.rgba(Kirigami.Theme.negativeTextColor.r, Kirigami.Theme.negativeTextColor.g, Kirigami.Theme.negativeTextColor.b, 0.2)

                }

            ]
        }
    }

    initialItem: Maui.Page
    {
        padding: control.depth === 2 ? 0 : Maui.Style.space.big

        headBar.rightContent: [
            ToolButton
            {
                icon.name: "edit-find"
            },

            ToolButton
            {
                icon.name: "view-sort"
            }
        ]

        ListView
        {
            id: _listView
            anchors.fill: parent
            orientation: ListView.Vertical
            spacing: Maui.Style.space.medium
            section.property: "category"
            section.criteria: ViewSection.FullString
            section.delegate: Maui.LabelDelegate
            {
                label: section
                labelTxt.font.pointSize: Maui.Style.fontSizes.big
                isSection: true
                labelTxt.font.bold: true
                height: Maui.Style.toolBarHeightAlt
                width: parent.width
            }
            model: _progressManager

            delegate: PackageDelegate
            {
                id: _delegate

                height: 140
                width: parent.width - Maui.Style.space.huge
                anchors.horizontalCenter: parent.horizontalCenter

            }
        }
    }
}
