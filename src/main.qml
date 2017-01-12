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

//import Snapd 1.0


ApplicationWindow {
    visible: true
    width: 640
    height: 400
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
                checked: content.currentItem
                         && content.currentItem.objectName == "homeView"
                onClicked: {
                    content.clear()
                    content.push(homeView)
                }
            }
            PlasmaComponents.Button {
                iconName: "application"
            }
            PlasmaComponents.Button {
                iconName: "favorites"
                checked: content.currentItem
                         && content.currentItem.objectName == "changesView"
                onClicked: {
                    content.clear()
                    content.push(changesView)
                }
            }
            PlasmaComponents.Button {
                iconName: "edit-download"
                checked: content.currentItem
                         && content.currentItem.objectName == "workView"
                onClicked: {
                    content.clear()
                    content.push(workView)
                }
            }

            PlasmaComponents.TextField {
                id: searchField
                Layout.fillWidth: true
                Layout.leftMargin: 60
                Layout.rightMargin: 60
                placeholderText: "Search"
            }

            PlasmaComponents.Button {
                Layout.alignment: Qt.AlignRight
                iconName: "configure"
            }
        }

        StackView {
            id: content
            Layout.fillWidth: true
            Layout.fillHeight: true

            initialItem: HomeView {}
        }
    }



}
