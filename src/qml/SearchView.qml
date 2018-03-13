import QtQuick 2.0
import QtQuick.Layouts 1.3

import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.nxos.softwarecenter 1.0

import "parts" as Parts

Item {
    GridView {
        id: appImageHubListView
        width: Math.floor( (parent.width - (anchors.leftMargin + anchors.rightMargin)) / cellWidth) * cellWidth
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.leftMargin: 18
        anchors.rightMargin: 12

        clip: true

        model: SearchController.model

        cellWidth: 200
        cellHeight: 200

        delegate: ApplicationGridItemDelegate {
            id: applicationGridItemDelegate
            name: app_name
            version: app_version
            icon: app_icon

            hasPendingAction: TasksController.affectedApplicationsIds.indexOf(
                                  app_id) > -1
            installed: RegistryController.installedApplications.indexOf(app_id) > -1
            upgradable: UpgraderController.upgradableApplications.indexOf(
                            app_code_name) > -1

            onRequestGet: InstallController.install(app_id)
            onRequestRemove: UninstallController.uninstall(app_id)
            onRequestUpgrade: UpgraderController.upgrade(app_code_name)
            onRequestRun: RunController.run(app_id)
            onRequestView: {
                ApplicationViewController.loadApplication(app_id)
                showApplicationView(app_name)
            }
        }
    }
}

