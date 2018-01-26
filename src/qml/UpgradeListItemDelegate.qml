import QtQuick 2.0
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

PlasmaComponents.ListItem {
    height: Math.max(38, innerLayout.height)

    property alias changeslog_message: changesLogMessage.text

    property alias icon: icon.source
    property alias name: labelName.text
    property alias version: labelVersion.text

    signal requestUpgrade()

    RowLayout {
        id: innerLayout

        anchors.fill: parent
        PlasmaCore.IconItem {
            id: icon
            Layout.maximumWidth: 180
            Layout.preferredHeight: 100
            Layout.maximumHeight: 120
            Layout.alignment: Qt.AlignCenter
        }

        ColumnLayout {
            Layout.maximumWidth: 300
            Layout.preferredWidth: 300
            Layout.leftMargin: 8
            Layout.fillWidth: true
            PlasmaComponents.Label {
                id: labelName
                Layout.fillWidth: true
                elide: Text.ElideRight

                font.bold: true
            }

            PlasmaComponents.Label {
                id: labelVersion
                Layout.fillWidth: true
                elide: Text.ElideRight

                visible: text != ""
            }
        }

        PlasmaComponents.Label {
            id: changesLogMessage
            Layout.leftMargin: 8
            Layout.fillWidth: true

            wrapMode: Text.WordWrap

            visible: text != ""
        }


        PlasmaComponents.Button {
            Layout.alignment: Qt.AlignRight
            Layout.leftMargin: 8
            text: i18n("Upgrade")

            onClicked: {
                enabled = false
                requestUpgrade()
            }
        }
    }
}
