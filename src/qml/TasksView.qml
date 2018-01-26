import QtQuick 2.0
import QtQuick.Layouts 1.3

import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.nxos.softwarecenter 1.0

Item {
    ColumnLayout {
        id: tasksViewRoot

        anchors.fill: parent
        anchors.margins: 12

        PlasmaExtras.Heading {
            text: "Running Tasks"
            visible: tasksScrollArea.visible
        }
        PlasmaExtras.ScrollArea {
            id: tasksScrollArea
            visible: TasksController.model.rowCount() > 0
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: tasksListView

                clip: true

                model: TasksController.model
                spacing: 12
                delegate: TaskListItemDelegate {
                    onRequestCancel: TasksController.cancelTask(task_id)
                    app_icon: "package-x-generic"
                    app_name: task_application_name
                    app_author: task_application_author

                    progress_value: task_progress_value
                    progress_total: task_progress_total
                    progress_message: task_progress_message
                }
            }
        }
        PlasmaExtras.Heading {
            text: "Updates"
            visible: upgradesScrollArea.visible
        }

        PlasmaExtras.ScrollArea {
            id: upgradesScrollArea
            visible: UpgraderController.model.rowCount() > 0
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: upgradesListView
                model: UpgraderController.model
                delegate: UpgradeListItemDelegate {
                    icon: "package-x-generic"
                    name: new_app_name
                    version: new_app_version

                    changeslog_message: "Newer version available"

                    onRequestUpgrade: UpgraderController.upgrade(old_app_id, new_app_id);
                }
            }
        }

        RowLayout {
            visible: registryScrollArea.visible

            PlasmaExtras.Heading {
                Layout.fillWidth: true
                text: "History"
            }

            PlasmaComponents.Button {
                Layout.rightMargin: 18
                iconName: "trash-empty"
                onClicked: RegistryController.clearRecords()
            }
        }

        PlasmaExtras.ScrollArea {
            id: registryScrollArea
            visible: RegistryController.model.rowCount() > 0
            Layout.fillWidth: true
            Layout.fillHeight: true

            ListView {
                id: recordsListView
                model: RegistryController.model
                delegate: RecordListItemDelegate {
                    app_icon: "package-x-generic"
                    app_name: task_application_name
                    app_version: task_application_version

                    record_timestamp: timestamp
                    record_message: message
                }
            }
        }
    }
}
