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
            model: DeployedApplicationsController.applications

            delegate: ApplicationGridItemDelegate {
                id: applicationGridItemDelegate

                width: appImageHubListView.cellWidth
                height: appImageHubListView.cellWidth

                Layout.alignment: Qt.AlignTop | Qt.AlignLeft

                name: modelData["name"]
                version: modelData["latestReleaseVersion"]
                icon: modelData["icon"]
                size: modelData["fileSize"]

                hasPendingAction: TasksController.affectedApplicationsIds.indexOf(modelData["id"]) > -1
                upgradable: UpgraderController.upgradableApplicationIds.indexOf(modelData["id"]) > -1

                deployed: true

                onRequestRemove: RemoveController.removeAppImage(modelData["filePath"])
                onRequestUpgrade: UpgraderController.upgradeApplication(modelData["id"])
                onRequestRun: RunController.runAppImage(modelData["filePath"])
                onRequestView: {
                    ApplicationViewController.loadApplication(modelData["id"])
                    showApplicationView(modelData["name"])
                }
            }
        }
    }
}
