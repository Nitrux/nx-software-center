import QtQuick 2.0
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.nxos.softwarecenter 1.0

import "parts" as Parts

Item {
    id: searchViewRoot

    PlasmaExtras.ScrollArea {
        anchors.fill: parent
        anchors.topMargin: 12
        anchors.bottomMargin: 6
        anchors.rightMargin: 6

        ListView {
            id: appImageHubListView

            clip: true

            model: SearchController.model
            spacing: 12
            delegate: AppImageListItemDelegate {
                isDownloaded: Registry.isDownloaded(app_id)
                onRequestInstall: InstallController.install(app_id)
            }
/*
            delegate: AppImageListItemDelegate {
                codeName: model.modelData['codeName']
                description: model.modelData['description']

                onRequestDownload: {
                    taskId = TasksController.download(
                                model.modelData['id'],
                                model.modelData['latest_release_id'])
                    task = TasksController.getTask(taskId)
                }

                onRequestRemove: {
                    taskId = TasksController.remove(
                                model.modelData['id'],
                                model.modelData['latest_release_id'])
                    task = TasksController.getTask(taskId)
                }

                onRequestExecute: {
                    taskId = TasksController.execute(
                                model.modelData['id'],
                                model.modelData['latest_release_id'])
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
                    isDownloaded = RegistryController.isReleaseDownloaded(
                                model.modelData['id'],
                                model.modelData['latest_release_id'])
                }
            }
*/
        }
    }
}
