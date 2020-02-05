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

//    property alias category : _appsList.category

    Maui.BaseModel
    {
        id: _appsModel
        list: NX.Apps
        {
            id: _appsList
        }
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

        headBar.middleContent: [
            Maui.PathBar
            {
                Layout.fillWidth: true
                url: _appsList.category.name

                Maui.TextField
                {
                    visible: false
                }
            }
        ]

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

        Maui.Holder
        {
            title: qsTr("Oops!")
            body: qsTr("No apps found")
            visible: _appsListView.count === 0
            emoji: "qrc:/store.svg"
            emojiSize: Maui.Style.iconSizes.huge
        }

        ListView
        {
            id: _appsListView
            anchors.fill: parent
            orientation: ListView.Vertical
            spacing: Maui.Style.space.medium
//            currentIndex: -1
            section.property: "category"
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

            model: _appsModel

            delegate: Maui.SwipeBrowserDelegate
            {
                id: _delegate
                height: 100
                width: parent.width
                anchors.horizontalCenter: parent.horizontalCenter
                label1.text: model.name
                label2.text: model.description
                iconSource: model.smallpic
                iconSizeHint: height * 0.7

                onClicked: console.log("JAJAJAJ")

                quickActions: [
                    Action
                    {
                        icon.name: "media-playback-start"
                        onTriggered: {
                            console.log(">>>>", model.path)
                            _appsList.launchApp(model.path)
                        }
                    },
                    Action
                    {
                        icon.name: "entry-delete"
                    }
                ]
            }
        }
    }
}
