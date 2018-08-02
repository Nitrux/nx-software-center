import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.3

import org.nxos.softwarecenter 1.0

Item {
    height: childrenRect.height

    GridLayout {
        id: appImageHubListView

        property int cellWidth: 200
        columns: Math.floor(
                     (parent.width - (anchors.leftMargin + anchors.rightMargin)) / cellWidth)
        width: Math.min(columns, SearchController.model.rowCount()) * cellWidth

        anchors.top: parent.top
        anchors.left: parent.left
        anchors.leftMargin: 28
        anchors.rightMargin: 12

        Repeater {
            model: SearchController.model

            delegate: ApplicationGridItemDelegate {
                id: applicationGridItemDelegate

                width: appImageHubListView.cellWidth
                height: appImageHubListView.cellWidth

                Layout.alignment: Qt.AlignTop | Qt.AlignLeft

                name: app_name
                version: app_version
                icon: app_icon

                hasPendingAction: TasksController.affectedApplicationsIds.indexOf(app_id) > -1
                deployed: DeployedApplicationsController.applicationsIds.indexOf(app_id) > -1
                upgradable: UpgraderController.upgradableApplicationIds.indexOf(app_id) > -1

                onRequestGet: DeployController.deploy(app_id)
                onRequestRemove: RemoveController.remove(app_id)
                onRequestUpgrade: UpgraderController.upgradeApplication(app_id)
                onRequestRun: RunController.run(app_id)
                onRequestView: {
                    ApplicationViewController.loadApplication(app_id)
                    showApplicationView(app_name)
                }
            }
        }
    }
}
