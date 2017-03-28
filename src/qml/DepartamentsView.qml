import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import Snapd 1.0

import org.nx.softwarecenter 1.0
import "qrc:/scripts/Utils.js" as UtilsJs

import "parts"
import "interactors" as Interactors

Item {
    id: departamentsRoot
    property var departamentsListModel

    property string currentDepartamentSlug: ""

    ListView {
        id: departamentsListView
        anchors {
            top: parent.top
            left: parent.left
            leftMargin: 12
            right: parent.right
            rightMargin: 6
        }
        height: 200

        focus: true
        property int viewIndex: 0
        function moveViewportForward() {
            flick(width * -2, 0)
        }
        function moveViewportBackward() {
            flick(width * 2, 0)
        }

        //        keyNavigationEnabled: true
        //        snapMode: ListView.SnapToItem
        spacing: 12
        orientation: ListView.Horizontal
        highlightFollowsCurrentItem: true
        highlight: Item {
        }
        model: departamentsListModel
        delegate: Card {
            width: 160
            height: 200
            clip: true

            color: index == departamentsListView.currentIndex ? "#aae3ff" : "white"
            Image {
                width: 160
                height: 160
                source: "file:///usr/share/nx_software_center/departaments_background/"
                        + model.slug + ".png"
                fillMode: Image.PreserveAspectCrop
                opacity: index == departamentsListView.currentIndex ? 1 : 0.7
            }

            Text {
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.margins: 12
                text: model.name
            }
            MouseArea {
                anchors.fill: parent
                onClicked: departamentsListView.currentIndex = index
            }
        }
    }

    Item {
        anchors.left: departamentsListView.left
        anchors.verticalCenter: departamentsListView.verticalCenter
        //        anchors.horizontalCenterOffset: 2
        height: 50
        width: height

        Rectangle {
            anchors.fill: parent
            radius: height
            color: "white"
            opacity: 0.7
        }

        PlasmaCore.IconItem {
            anchors.centerIn: parent
            source: "arrow-left"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: departamentsListView.moveViewportBackward()
        }
    }

    Item {
        anchors.right: departamentsListView.right
        anchors.verticalCenter: departamentsListView.verticalCenter
        //        anchors.horizontalCenterOffset: 2
        height: 50
        width: height

        Rectangle {
            anchors.fill: parent
            radius: height
            color: "white"
            opacity: 0.7
        }

        PlasmaCore.IconItem {
            anchors.centerIn: parent
            source: "arrow-right"
        }

        MouseArea {
            anchors.fill: parent
            onClicked: departamentsListView.moveViewportForward()
        }
    }


    Interactors.ListSnapsInDepartamentInteractor {
        departament: departamentsListModel && departamentsListView.currentIndex >= 0 ? departamentsListModel.get(departamentsListView.currentIndex).slug : ""
        onDepartamentChanged: {
            print(departament)
            fetchSnaps()
        }
        onLoading: {
            snapsListLoader.source = "qrc:/PlaceHolderView.qml"
            var placeHolder = snapsListLoader.item
            if (placeHolder !== undefined) {
                placeHolder.message = i18n("Loading snaps")
                placeHolder.showBusyIndicator = true
            }
        }
        onFinished: {
            if (snaps.length == 0) {
                snapsListLoader.source = "qrc:/PlaceHolderView.qml"
                var placeHolder = snapsListLoader.item
                if (placeHolder !== undefined) {

                    placeHolder.message = textConstants.noSnapsFound
                    placeHolder.iconName = "face-sad"
                    placeHolder.showBusyIndicator = false
                }
            } else {
                snapsListLoader.sourceComponent = departamentSnaps
                var snapsGrid = snapsListLoader.item
                if (snapsGrid !== undefined) {
                    snapsGrid.model.clear()
                    for (var i in snaps)
                        snapsGrid.model.append(snaps[i])
                }
            }
        }
        onError: {
            snapsListLoader.source = "qrc:/PlaceHolderView.qml"
            var placeHolder = contentLoader.item
            if (placeHolder !== undefined) {
                var message = listDepartamentSnapsRequest.errorString
                if (message == "")
                    message = textConstants.unknownError

                placeHolder.message = message
                placeHolder.iconName = "face-uncertain"
                placeHolder.showBusyIndicator = false
            }
        }
    }

    Loader {
        id: snapsListLoader
        anchors {
            top: split.top
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }
    }

    Rectangle {
        id: split
        anchors.top: departamentsListView.bottom
        anchors.topMargin: 12
        anchors.left: parent.left
        anchors.right: parent.right
        height: 6

//        color: "lightgray"
        gradient: Gradient {
            GradientStop {
                position: 0.0
                color: "#AFAFB1"
            }
            GradientStop {
                position: 1.0
                color: "transparent"
            }
        }
    }

    Component {
        id: departamentSnaps
        SnapGrid {
            model: SnapsModel {
                id: storeSnapsModel

                property var snaps
                onSelectedItemsChanged: {
                    refreshActions()
                }

                function _indexOf(name) {
                    return UtilsJs.binaryIndexOf(name, function (a, b) {
                        return a.package_name.localeCompare(b)
                    }, function () {
                        return count
                    }, function (i) {
                        return get(i)
                    })
                }

                function refreshActions() {
                    var keys = Object.keys(selectedItems)
                    if (keys.length > 0) {
                        var installAction = {
                            icon: "package-install",
                            text: textConstants.actionInstallTitle,
                            action: function () {
                                var targets_names = Object.keys(
                                            storeSnapsModel.selectedItems)
                                var targets = []
                                for (var i in targets_names) {
                                    var model = storeSnapsModel.getByName(
                                                targets_names[i])
                                    targets.push({
                                                     name: model.package_name,
                                                     channel: model.channel
                                                 })
                                }

                                installSnapInteractor.targets = targets
                                installSnapInteractor.finished.connect(
                                            refreshActions)

                                installSnapInteractor.start()
                            }
                        }

                        var actions = [installAction]
                        statusArea.updateContext(
                                    "documentinfo",
                                    textConstants.availableActionsNotice,
                                    actions)
                    } else
                        statusArea.clearContext()
                }
            }
            delegate: SnapElementDelegate {
                snap_name: title
                snap_version: version
                snap_size: download_size
                onSelectedChanged: {
                    for (var k in model)
                        print(k, model[k])
                    if (selected)
                        storeSnapsModel.selectedItems[package_name] = "true"
                    else
                        delete storeSnapsModel.selectedItems[package_name]

                    storeSnapsModel.refreshActions()
                }
                onClicked: main.showSnapDetails(package_name);
            }
        }
    }

    Component.onCompleted: statusArea.clearContext()
}
