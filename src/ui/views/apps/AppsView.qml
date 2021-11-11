import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3

import org.kde.kirigami 2.7 as Kirigami
import org.mauikit.controls 1.3 as Maui

import NXModels 1.0 as NX
import "../../templates"

Maui.Page
{
    id: control

    property alias list : _appsList
    required property var taskManager

    Maui.Dialog
    {
        id: appLaunchErrorDialog

        title: i18n("Error")
        message: i18n("Error launching application")
        rejectButton.visible: false

        acceptButton.onClicked:
        {
            appLaunchErrorDialog.visible = false;
        }

        function showDialog(fileName, errCode)
        {
            switch (errCode)
            {
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

    Maui.Dialog
    {
        id: appUpdateDialog

        title: i18n("AppImage Update")
        message: i18n("Please wait...")
        rejectButton.visible: false

        acceptButton.onClicked:
        {
            appUpdateDialog.visible = false;
        }

        function showDialog(fileName, status)
        {
            message = status

            appUpdateDialog.visible = true;
        }
    }

    Maui.Dialog
    {
        id: appRemoveDialog

        property int index : -1

        title: i18n("Remove")
        message: i18n("Are you sure you want to remove this application?")
        template.iconSource: "emblem-warning"
        page.margins: Maui.Style.space.big
        spacing: Maui.Style.space.medium
        onRejected: appRemoveDialog.close()
        onAccepted:
        {
            _appsList.removeApp(index);
        }
    }

    headBar.forceCenterMiddleContent: isWide
    headBar.middleContent: Maui.TextField
    {
        id: _filterBar
        Layout.fillWidth: true
        Layout.maximumWidth: 500
        placeholderText: i18n("Filter %1 installed apps", _appsList.count)
        onAccepted: _appsModel.filter = text
        onCleared:  _appsModel.filter = ""
    }

    headBar.rightContent: [

        Maui.ToolButtonMenu
        {
            icon.name: "view-sort"

            MenuItem
            {
                text: i18n("Name")
                checkable: true
                autoExclusive: true
                checked: _appsModel.sort === "label"
                onTriggered: _appsModel.sort = "label"
            }

            MenuItem
            {
                text: i18n("Size")
                checkable: true
                autoExclusive: true
                checked: _appsModel.sort === "size"
                onTriggered: _appsModel.sort = "size"

            }

            MenuItem
            {
                text: i18n("Date")
                checkable: true
                autoExclusive: true
                checked: _appsModel.sort === "date"
                onTriggered: _appsModel.sort = "date"
            }

            MenuItem
            {
                text: i18n("Category")
                checkable: true
                autoExclusive: true
                checked: _appsModel.sort === "category"
                onTriggered: _appsModel.sort = "category"
            }
        }
    ]

    Maui.Holder
    {
        anchors.fill: parent
        title: i18n("Oops!")
        body: i18n("No apps found")
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
        section.property: _appsModel.sort
        section.criteria:  _appsModel.sort === "label" ? ViewSection.FirstCharacter : ViewSection.FullString
        section.delegate: Maui.LabelDelegate
        {
            id: delegate
            label: _appsModel.sort === "date" ?  Qt.formatDateTime(new Date(section), "d MMM yyyy") : ( _appsModel.sort === "size" ?  Maui.Handy.formatSize(String(section)) :  String(section).toUpperCase())

            isSection: true

            height: Maui.Style.toolBarHeightAlt
            width: parent.width
        }

        model: Maui.BaseModel
        {
            id: _appsModel
            sort: "label"
            sortOrder: Qt.AscendingOrder
            recursiveFilteringEnabled: true
            sortCaseSensitivity: Qt.CaseInsensitive
            filterCaseSensitivity: Qt.CaseInsensitive
            list: NX.Apps
            {
                id: _appsList
                taskManager: control.taskManager
            }
        }

        delegate: Maui.SwipeBrowserDelegate
        {
            height: 64
            width: parent.width
            anchors.horizontalCenter: parent.horizontalCenter
            label1.text: model.label
            label2.text: model.name
            imageSource: "image://thumbnailer/" + model.path
            iconSizeHint: Maui.Style.iconSizes.medium
            iconSource: "package"

            onClicked:
            {
                _appsListView.currentIndex = index;

                if(Maui.Handy.singleClick || Kirigami.Settings.hasTransientTouchInput)
                {
                    _appsList.launchApp(_appsModel.mappedToSource(index));
                }
            }

            onDoubleClicked:
            {
                _appsListView.currentIndex = index;

                if(!Maui.Handy.singleClick)
                {
                    _appsList.launchApp(_appsModel.mappedToSource(index));
                }
            }

            quickActions: [
                Action
                {
                    icon.name: "media-playback-start"
                    onTriggered:
                    {
                        _appsListView.currentIndex = index;
                        _appsList.launchApp(_appsModel.mappedToSource(index));
                    }
                },
                Action
                {
                    icon.name: "download"
                    // enabled: _appsList.isUpdatable
                    onTriggered:
                    {
                        taskManagerCtx.doUpdate(model.url, model.name);
                    }
                },
                Action
                {
                    icon.name: "entry-delete"
                    onTriggered:
                    {
                        _appsListView.currentIndex = index;
                        appRemoveDialog.index = _appsModel.mappedToSource(index)
                        appRemoveDialog.open()
                    }
                }
            ]
        }
    }

    Connections
    {
        target: _appsList

        function onAppLaunchError(err) {
            console.log("App Launch Error", err);
            appLaunchErrorDialog.showDialog(_appsListView.model.get(_appsListView.currentIndex).path.split("/").pop(), err);
            _appsListView.currentIndex = -1;
        }

        function onAppLaunchSuccess() {
            _appsListView.currentIndex = -1;
        }

        function onAppDeleteSuccess() {
            appRemoveDialog.visible = false;
        }

        function onAppUpdateSuccess(msg) {
            console.log("AppImage updated successfully.");
            
            appUpdateDialog.showDialog(_appsListView.model.get(_appsListView.currentIndex).path.split("/").pop(), 
                msg);
        }

        function onAppUpdateError(err) {
            console.log("AppImage update error.");
            appUpdateDialog.showDialog(_appsListView.model.get(_appsListView.currentIndex).path.split("/").pop(),
                err);
        }
    }
}



