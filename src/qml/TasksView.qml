import QtQuick 2.0
import QtQuick.Layouts 1.3

import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.nxos.softwarecenter 1.0

Flickable {
    contentHeight: tasksViewRoot.height

    flickableDirection: Flickable.VerticalFlick
    ColumnLayout {
        id: tasksViewRoot
        width: parent.width > 1000 ? 976 : parent.width - 60;
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 12


        PlasmaExtras.Heading {
            text: i18n("Downloads")
            Layout.topMargin: 18
            visible: tasksListView.count
            level: 5
        }

        Repeater {
            id: tasksListView
            Layout.bottomMargin: 18
            visible: count

            model: TasksController.model
            delegate: TaskItem {
                onRequestCancel: TasksController.cancelTask(task_id)
                app_name: task_application_name
                app_icon: task_application_icon
                app_author: task_application_author

                progress_value: task_progress_value
                progress_total: task_progress_total
                progress_message: task_progress_message
            }


        }

        PlasmaExtras.Heading {
            text: i18n("Updates available")
            Layout.topMargin: 18
            visible: upgradesListView.count
            level: 5
        }

        Repeater {
            id: upgradesListView
            model: UpgraderController.upgrades
            delegate: UpgradeItem {
                icon: modelData["app_icon"]
                name: modelData["app_name"]
                version: modelData["release_version"]

                changeslog_message: modelData["release_changelog"]

                onRequestUpgrade: UpgraderController.upgradeApplication(modelData["app_id"])
                dontHavePendingTasks: TasksController.affectedApplicationsIds.indexOf(modelData["app_id"]) === -1
            }

            Layout.bottomMargin: 18
        }


        PlasmaExtras.Heading {
            Layout.preferredHeight: 200
            Layout.alignment: Qt.AlignCenter
            Layout.bottomMargin: 18

            text: i18n("No updates found.")

            visible: !upgradesListView.count
            level: 5
        }

        RowLayout {
            Layout.topMargin: 18
            Layout.fillWidth: true
            visible: recordsListView.count

            PlasmaExtras.Heading {
                Layout.fillWidth: true
                text: i18n("Recent changes")
                level: 4
            }

            PlasmaComponents.Button {
                Layout.rightMargin: 18
                iconName: "trash-empty"
                onClicked: TaskLoggerController.clearRecords()
            }
        }

        Repeater {
            id: recordsListView
            model: TaskLoggerController.model
            delegate: TaskRecordItem {
                Layout.topMargin: 8
                app_icon: "package-x-generic"
                app_name: task_application_name
                app_version: task_application_version

                record_timestamp: timestamp
                record_message: message
            }
        }
    }

    // Togle tasks notifications for this view
    Component.onCompleted: NotificationsController.tasksNotificationsEnabled = false
    Component.onDestruction: NotificationsController.tasksNotificationsEnabled = true
}
