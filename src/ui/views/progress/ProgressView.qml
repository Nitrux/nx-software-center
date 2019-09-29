import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.kde.mauikit 1.0 as Maui

import "../../templates"

StackView
{
    id: control

    property bool isActive: false

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
            anchors.fill: parent
            orientation: ListView.Vertical
            spacing: Maui.Style.space.medium
            section.property: "category"
            section.criteria: ViewSection.FullString
            section.delegate: Maui.LabelDelegate
            {
                id: delegate
                label: section
                labelTxt.font.pointSize: Maui.Style.fontSizes.big
                isSection: true
                boldLabel: true
                height: Maui.Style.toolBarHeightAlt
            }
            model: ListModel
            {
                ListElement{ category: "Downloading"; label: "All"; icon: "appimage-store"; count: 72000; itemInfo: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper."}
                ListElement{ category: "Downloading"; label: "Education"; icon: "applications-education"; count: 650; itemInfo: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper."}
                ListElement{ category: "Downloading"; label: "Development"; icon: "applications-development"; count: 300; itemInfo: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper."}
                ListElement{ category: "Downloading"; label: "Graphics"; icon: "applications-graphics"; count: 13}
                ListElement{ category: "Updating"; label: "Internet"; icon: "applications-internet"; itemInfo: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper."}
                ListElement{ category: "Updating"; label: "Games"; icon: "applications-games"; count: 7}
                ListElement{ category: "Updating"; label: "Multimedia"; icon: "applications-multimedia"; itemInfo: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper."}
                ListElement{ category: "Removing"; label: "Office"; icon: "applications-office"; itemInfo: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper."}
            }

            delegate: CardDelegate
            {
                id: _delegate
                height: 100
                width: control.width - Maui.Style.space.huge
                anchors.horizontalCenter: parent.horizontalCenter
                label1.text: model.label
                label2.text: model.itemInfo
                label3.text: model.count
                iconImg.source: model.icon

                quickButtons: [

                    ProgressBar
                    {
                      implicitWidth: 250
                      anchors.verticalCenter: parent.verticalCenter
                    },

                    ToolButton
                    {
                        icon.name: "process-stop"
                    },

                    ToolButton
                    {
                        icon.name: "media-playback-start"
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
