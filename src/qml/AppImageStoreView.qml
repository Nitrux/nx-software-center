import QtQuick 2.0
import QtQuick.Layouts 1.3

import org.nx.softwarecenter 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras

import "parts" as Parts

Parts.View {
    id: appImageStoreViewRoot

    objectName: "appImageStoreView"
    function query(text) {
        SearchViewController.search(text)
    }

    Component.onCompleted: {
        print(query())
        query("")
    }

    Connections {
        target: SearchViewController
        onApplications: appImageHubListView.model = apps
        onNoMatchFound: {
            appImageHubListView.model = undefined
        }
    }

    Parts.Card {
        id: wipNotice

        height: 70

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 12

        RowLayout {
            anchors.fill: parent
            anchors.margins: 12

            PlasmaCore.IconItem {
                source: "dialog-information"
                height: 64
            }

            PlasmaExtras.Heading {
                Layout.fillWidth: true
                Layout.leftMargin: 12
                wrapMode: Text.WordWrap

                level: 4

                text: "AppImages listing in this store is a work in progress held in coordination with the AppImage development team. New updates will be published soon."
            }
        }


    }

    PlasmaExtras.ScrollArea {
        anchors.top: wipNotice.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        anchors.topMargin: 12
        anchors.bottomMargin: 6
        anchors.rightMargin: 6

        ListView {
            id: appImageHubListView

            clip: true

            spacing: 12

            delegate: AppImageListItemDelegate {
                name: model.modelData['name']
                description: model.modelData['description']

                onRequestDownload: {
                    taskId = TasksController.download(model.modelData['id'], model.modelData['latest_release_id'])
                    task = TasksController.getTask(taskId)
                }

                onRequestRemove: {
                    taskId = TasksController.remove(model.modelData['id'], model.modelData['latest_release_id'])
                    task = TasksController.getTask(taskId)
                }

                onRequestExecute: {
                    taskId = TasksController.execute(model.modelData['id'], model.modelData['latest_release_id'])
                    task = TasksController.getTask(taskId)
                }

                onRequestRefresh: loadData()
                Component.onCompleted: loadData()
                Component.onDestruction: {
                    taskId = ""
                    task = null
                }

                function loadData() {
                    isDownloadable = model.modelData['download_link']
                    isDownloaded = RegistryController.isReleaseDownloaded(model.modelData['id'], model.modelData['latest_release_id'])
                }
            }
        }
    }

}
