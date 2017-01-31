import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.nx.softwarecenter 1.0

import "qrc:/actions/ApplySettingsAction.js" as ApplySettingsAction

Item {
    id: settingsViewRoot
    objectName: "settingsView"

    Component.onCompleted: {
        SnapdSettings.load()

        var actions = [ApplySettingsAction.prepare(SnapdSettings)]
        statusArea.updateContext("documentinfo", i18n("Available actions"), actions)
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 18

        PlasmaComponents.Label {
            text: i18n("Snaps Store")
        }
        GroupBox {

            ColumnLayout {
                ExclusiveGroup {
                    id: storeGroup
                }
                PlasmaComponents.RadioButton {
                    id: ubuntuStoreRadio
                    text: i18n("Ubuntu Store")
                    exclusiveGroup: storeGroup
                    checked: SnapdSettings.store == SnapdSettings.Ubuntu
                    onCheckedChanged: if (checked)
                                          SnapdSettings.store = SnapdSettings.Ubuntu
                }

                PlasmaComponents.RadioButton {
                    id: customStoreRadio
                    text: i18n("Custom Store")
                    exclusiveGroup: storeGroup
                    checked: SnapdSettings.store == SnapdSettings.Custom
                    onCheckedChanged: if (checked)
                                          SnapdSettings.store = SnapdSettings.Custom
                }
                PlasmaComponents.TextField {
                    Layout.leftMargin: 40
                    Layout.preferredWidth: 400

                    enabled: customStoreRadio.checked
                    placeholderText: "http://mynapstore.com"
                    text: SnapdSettings.customStoreUrl

                    onTextChanged: SnapdSettings.customStoreUrl = text
                }
            }
        }

        PlasmaComponents.Switch {
            id: useProxyRadio
            text: i18n("Proxy")

            checked: SnapdSettings.useProxy

            onCheckedChanged: SnapdSettings.useProxy = checked
        }

        GroupBox {
            enabled: useProxyRadio.checked
            ColumnLayout {
                PlasmaComponents.Label {
                    text: i18n("HTTP")
                }

                PlasmaComponents.TextField {
                    Layout.preferredWidth: 200
                    text: SnapdSettings.httpProxy

                    onTextChanged: SnapdSettings.httpProxy = text
                }

                PlasmaComponents.Label {
                    text: i18n("HTTPS")
                }

                PlasmaComponents.TextField {
                    Layout.preferredWidth: 200
                    text: SnapdSettings.httpsProxy

                    onTextChanged: SnapdSettings.httpsProxy = text
                }

                PlasmaComponents.Label {
                    text: i18n("Ignore")
                }

                PlasmaComponents.TextField {
                    Layout.preferredWidth: 200
                    text: SnapdSettings.noProxy

                    onTextChanged: SnapdSettings.noProxy = text
                }
            }
        }
    }
}
