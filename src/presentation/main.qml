import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import QtQuick.Controls.Styles 1.4
import QtQuick.Controls 1.2 as QtControls
import QtQuick.Controls.Styles.Plasma 2.0 as Styles

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import QtQuick.Controls.Styles.Plasma 2.0 as Styles
import org.kde.plasma.extras 2.0 as PlasmaExtras

import org.nx.softwarecenter 1.0

import Snapd 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: qsTr("NX Software Center")

    ColumnLayout {
        anchors.fill: parent
        RowLayout {
            Layout.leftMargin: 12
            Layout.rightMargin: 12
            Layout.topMargin: 6
            Layout.bottomMargin: 6
            Layout.fillWidth: true

            spacing: 8
            PlasmaComponents.Button {
                iconName: "go-home"
                checked: content.source == "qrc:/HomeView.qml"
                onClicked: {
                    if (checked)
                        return

                    content.source = "qrc:/HomeView.qml"
                }
            }
            PlasmaComponents.Button {
                iconName: "plasmadiscover"
                checked: content.source == "qrc:/StoreView.qml"
                onClicked: {
                    if (checked)
                        return
                    content.source = "qrc:/StoreView.qml"
                }
            }

            //            PlasmaComponents.Button {
            //                iconName: "edit-download"
            //                checked: content.currentItem
            //                         && content.currentItem.objectName == "workView"
            //                onClicked: {
            //                    content.clear()
            //                    content.push(workView)
            //                }
            //            }

            PlasmaComponents.TextField {
                id: searchField
                Layout.fillWidth: true
                Layout.leftMargin: 60
                Layout.rightMargin: 60
                placeholderText: "Search"
                focus: true

                onEditingFinished: {
                    if (content.source == "qrc:/StoreView.qml")
                        return

                    if (text == "")
                        return

                    content.source = "qrc:/StoreView.qml"
                }
            }

            PlasmaComponents.Button {
                Layout.alignment: Qt.AlignRight
                iconName: "configure"

                checked: content.source == "qrc:/SettingsView.qml"
                onClicked: {
                    if (checked)
                        return

                    content.source = "qrc:/SettingsView.qml"
                }
            }
        }

        Loader {
            id: content
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 400

            source : "qrc:/HomeView.qml"
        }

        //        StackView {
        //            id: content
        //            Layout.fillWidth: true
        //            Layout.fillHeight: true
        //            Layout.preferredHeight: 400

        //            initialItem: "qrc:/HomeView.qml"
        //        }

        StatusArea {
            id: statusArea
            Layout.maximumHeight: statusArea.visible ? 38 : 0
            Layout.preferredHeight: statusArea.visible ? 38 : 0
            Layout.fillWidth: true
            Layout.bottomMargin: 4
            Layout.rightMargin: 18

            onVisibleChanged: print(visible)
        }
    }
}
