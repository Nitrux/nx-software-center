import QtQuick 2.0
import QtQuick.Layouts 1.3
import QtQuick.Controls 1.4

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.nx.softwarecenter 1.0

import "qrc:/actions/ApplySettingsAction.js" as ApplySettingsAction
import "qrc:/actions/SetDefaultSettingsAction.js" as SetDefaultSettingsAction

Item {
    id: settingsViewRoot
    objectName: "settingsView"

    Component.onCompleted: {
        SnapdSettings.load()

        var actions = [SetDefaultSettingsAction.prepare(
                           SnapdSettings), ApplySettingsAction.prepare(
                           SnapdSettings)]
        statusArea.updateContext("documentinfo",
                                 i18n("Available actions"), actions)
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 18

        PlasmaComponents.Label {
            text: i18n("Select a store to get snaps from")
        }

        GroupBox {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            ColumnLayout {
                anchors.fill: parent

                ExclusiveGroup {
                    id: storeGroup
                }

                PlasmaComponents.RadioButton {
                    id: ubuntuStoreRadio
                    text: i18n("Ubuntu Store (default)")
                    exclusiveGroup: storeGroup
                    checked: SnapdSettings.store == SnapdSettings.Ubuntu
                    onCheckedChanged: if (checked)
                                          SnapdSettings.store = SnapdSettings.Ubuntu
                }

                PlasmaComponents.RadioButton {
                    id: customStoreRadio
                    text: i18n("Third-party Store")
                    exclusiveGroup: storeGroup
                    checked: SnapdSettings.store == SnapdSettings.Custom
                    onCheckedChanged: if (checked)
                                          SnapdSettings.store = SnapdSettings.Custom
                }

                RowLayout {
                    spacing: 12
                    Layout.fillWidth: true

                    PlasmaComponents.Label {
                        text: i18n("Enter url:")
                    }

                    PlasmaComponents.TextField {
                        Layout.fillWidth: true

                        enabled: customStoreRadio.checked
                        placeholderText: "http://mynapstore.com:5000/api/v1"
                        text: SnapdSettings.customStoreUrl

                        onTextChanged: SnapdSettings.customStoreUrl = text
                    }
                }
            }
        }

        PlasmaComponents.Label {
            text: i18n("Configure Proxy servers")
        }

        GroupBox {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignTop
            ExclusiveGroup {
                id: proxyGroup
            }

            ColumnLayout {
                anchors.fill: parent
                PlasmaComponents.RadioButton {
                    id: noProxyRadio
                    text: i18n("No Proxy")
                    exclusiveGroup: proxyGroup
                    checked: !SnapdSettings.useProxy
                }
                PlasmaComponents.RadioButton {
                    id: useProxyRadio
                    text: i18n("Use manually specified proxy")
                    exclusiveGroup: proxyGroup
                    checked: SnapdSettings.useProxy
                    onCheckedChanged: SnapdSettings.useProxy = checked
                }

                GridLayout {
                    columns: 4
                    enabled: useProxyRadio.checked
                    PlasmaComponents.Label {
                        text: i18n("HTTP Proxy")
                    }

                    PlasmaComponents.TextField {

                        Layout.fillWidth: true
                        text: SnapdSettings.httpProxy
                        placeholderText: "proxy1.com"

                        onTextChanged: SnapdSettings.httpProxy = text
                    }
                    PlasmaComponents.Label {
                        text: i18n("Port")
                    }

                    PlasmaComponents.TextField {
                        text: SnapdSettings.httpProxyPort
                        inputMethodHints: Qt.ImhPreferNumbers
                        validator: IntValidator {
                            bottom: 1
                            top: 65535
                        }
                        onTextChanged: SnapdSettings.httpProxyPort = text
                    }

                    PlasmaComponents.Label {
                        text: i18n("SSL Proxy")
                    }

                    PlasmaComponents.TextField {
                        Layout.fillWidth: true
                        text: SnapdSettings.httpsProxy
                        placeholderText: "proxy1.com"

                        onTextChanged: SnapdSettings.httpsProxy = text
                    }

                    PlasmaComponents.Label {
                        text: i18n("Port")
                    }

                    PlasmaComponents.TextField {
                        text: SnapdSettings.httpsProxyPort
                        inputMethodHints: Qt.ImhPreferNumbers
                        validator: IntValidator {
                            bottom: 1
                            top: 65535
                        }
                        onTextChanged: SnapdSettings.httpsProxyPort = text
                    }
                    PlasmaComponents.Label {
                        text: i18n("Ignore")
                    }

                    PlasmaComponents.TextField {

                        Layout.fillWidth: true
                        text: SnapdSettings.noProxy

                        onTextChanged: SnapdSettings.noProxy = text
                    }
                }
            }
        }
    }
}
