import QtQuick 2.0
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

Item {
    property alias icon: icon.source
    property alias name: labelName.text
    property alias version: labelVersion.text
    property alias size: labelSize.text

    property bool installed: false
    property bool upgradable: false
    property bool hasPendingAction: false

    signal requestRemove
    signal requestGet
    signal requestUpgrade

    height: 200
    width: 200

    PlasmaCore.FrameSvgItem {
        height: 180
        width: 180
        anchors.centerIn: parent

        imagePath: "opaque/widgets/panel-background"
    }

    ColumnLayout {
        id: contents
        height: 180
        width: 180
        anchors.centerIn: parent

        PlasmaCore.IconItem {
            id: icon
            Layout.maximumWidth: 180
            Layout.preferredHeight: 100
            Layout.maximumHeight: 120
            Layout.alignment: Qt.AlignCenter
        }

        RowLayout {
            Layout.alignment: Qt.AlignCenter
            Layout.maximumWidth: 180
            Layout.maximumHeight: 90
            ColumnLayout {
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

                PlasmaComponents.Label {
                    id: labelSize
                    Layout.fillWidth: true
                    elide: Text.ElideRight

                    visible: text != ""
                }
            }

            Loader {
                id: loader
                Layout.rightMargin: 8
                Layout.maximumWidth: 72
                Layout.preferredHeight: 36
                sourceComponent: installed ? removeButton : getButton
            }
        }

        Component {
            id: getButton
            PlasmaComponents.Button {
                enabled: !hasPendingAction
                text: upgradable ? i18n("Upgrade") : i18n("Get")

                onClicked: {
                    if (upgradable)
                        requestUpgrade()
                    else
                        requestGet()
                }
            }
        }

        Component {
            id: removeButton
            PlasmaComponents.Button {
                enabled: !hasPendingAction
                text: i18n("Remove")

                onClicked: requestRemove()
            }
        }
    }
}
