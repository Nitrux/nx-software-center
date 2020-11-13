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

    Maui.BaseModel
    {
        id: _appsModel
        list: NX.Apps
        {
            id: _appsList
        }
    }

    Maui.Dialog {
        id: appLaunchErrorDialog

        title: "Error"
        message: "Error launching application"
        rejectButton.visible: false

        acceptButton.onClicked: {
            appLaunchErrorDialog.visible = false;
        }

        function showDialog(fileName, errCode) {
            switch (errCode) {
                case 0:
                    message = "Error launching application <b>" + fileName + "</b>. Please check if you have correct permission"
                    break;

                default:
                    message = "Error launching application " + fileName
                    break;
            }

            appLaunchErrorDialog.visible = true;
        }
    }

    Maui.Dialog {
        property var filePath: ""

        id: appRemoveDialog

        title: "Remove"
        message: "Are you sure you want to remove this application?"

        acceptButton.onClicked: {
            _appsList.removeApp(filePath);
        }

        function showDialog(path) {
            appRemoveDialog.visible = true;
            filePath = path;
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

        Maui.ListBrowser
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
                height: 64
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
                            _appsListView.currentIndex = index;
                            _appsList.launchApp(model.path);
                        }
                    },
                    Action
                    {
                        icon.name: "entry-delete"
                        onTriggered: {
                            _appsListView.currentIndex = index;
                            appRemoveDialog.showDialog(model.path);
                        }
                    }
                ]
            }
        }
    }

    Connections {
        target: _appsList

        onAppLaunchError: {
            console.log("App Launch Error", err);
            appLaunchErrorDialog.showDialog(_appsListView.model.get(_appsListView.currentIndex).path.split("/").pop(), err);
            _appsListView.currentIndex = -1;
        }
        onAppLaunchSuccess: {
            _appsListView.currentIndex = -1;
        }

        onAppDeleteSuccess: {
            appRemoveDialog.visible = false;
        }
    }
}
