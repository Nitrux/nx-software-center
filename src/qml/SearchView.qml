import QtQuick 2.0
import QtQuick.Layouts 1.3

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

        GridView {
            id: appImageHubListView

            clip: true

            model: SearchController.model

            cellWidth: 200
            cellHeight: 200

            delegate: ApplicationGridItemDelegate {
                id: applicationGridItemDelegate
                icon: "package-x-generic"
                name: app_name
                version: app_version

                hasPendingAction: TasksController.affectedApplicationsIds.indexOf(app_id) > -1
                installed: RegistryController.installedApplications.indexOf(app_id) > -1
                upgradable: UpgraderController.upgradableApplications.indexOf(app_code_name) > -1

                onRequestGet: InstallController.install(app_id)
                onRequestRemove: UninstallController.uninstall(app_id)
                onRequestUpgrade: UpgraderController.upgrade(app_code_name)
            }
        }
    }
}
