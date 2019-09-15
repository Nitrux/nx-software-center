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

    property alias categoryUri : _appsList.categoryUri

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
                Layout.leftMargin: Maui.Style.space.small
                Layout.rightMargin: Maui.Style.space.small
                url: _appsList.categoryUri

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
            body: qsTr("Not apps found")
            visible: _appsListView.count === 0
            emoji: "qrc:/store.svg"
            emojiSize: Kirigami.Units.iconSizes.huge
        }

        ListView
        {
            id: _appsListView
            anchors.fill: parent
            orientation: ListView.Vertical
            spacing: Maui.Style.space.medium
            section.property: "category"
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

            model: _appsModel

            delegate: CardDelegate
            {
                id: _delegate
                height: 100
                width: control.width - Maui.Style.space.huge
                anchors.horizontalCenter: parent.horizontalCenter
                label1.text: model.name
                label2.text: model.description
                label3.text: model.count
                iconImg.source: model.icon

                quickButtons: [
                    ToolButton
                    {
                        icon.name: "document-share"
                    },

                    ToolButton
                    {
                        icon.name: "media-playback-start"
                    },

                    ToolButton
                    {
                        icon.name: "entry-delete"
                    }
                ]

                Connections
                {
                    target: _delegate
                    onClicked:
                    {
                        control.push(_appPageComponent)

                        //for testing the model this sia custom info model
                        var appInfo = {
                            name: "Index",
                            version: "1.0.0",
                            author: "Camilo Higuita",
                            organization: "Maui",
                            bannerImage: "qrc:/tests/banner_index.png",
                            iconName: "index",
                            iconImage: "",
                            downloadsToday: 34,
                            size: "125 MB",
                            updated: Date(),
                            license: "GPL v3",
                            itemInfo: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper. Curabitur fringilla rutrum mauris, in imperdiet dui commodo vitae. Fusce at enim ullamcorper, tincidunt orci a, tincidunt urna. Vivamus quis est ut elit dignissim semper. Vivamus erat leo, iaculis id faucibus ac, placerat eu lacus. Praesent arcu tortor, aliquet at volutpat eu, rutrum eget ante. Vivamus sed lorem sed massa interdum convallis. Suspendisse lacinia augue odio, id auctor ipsum varius id. Nam sit amet rhoncus augue. Vestibulum at lobortis velit. Proin ipsum eros, vehicula ut tempus vel, aliquam a orci. Fusce facilisis pulvinar orci. ",
                            changelogInfo: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper. Curabitur fringilla rutrum mauris, in imperdiet dui commodo vitae. Fusce at enim ullamcorper, tincidunt orci a, tincidunt urna. Vivamus quis est ut elit dignissim semper. Vivamus erat leo, iaculis id faucibus ac, placerat eu lacus. Praesent arcu tortor, aliquet at volutpat eu, rutrum eget ante. Vivamus sed lorem sed massa interdum convallis. Suspendisse lacinia augue odio, id auctor ipsum varius id. Nam sit amet rhoncus augue. Vestibulum at lobortis velit. Proin ipsum eros, vehicula ut tempus vel, aliquam a orci. Fusce facilisis pulvinar orci. ",
                            changelogVersion: "2.5.0"

                        }
                        control.currentItem.appInfo = appInfo
                    }
                }

            }
        }
    }
}
