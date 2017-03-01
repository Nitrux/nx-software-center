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

import "interactors" as Interactors

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: qsTr("NX Software Center")

    TextConstants {
        id: textConstants
    }

    ColumnLayout {
        anchors.fill: parent
        NavigationPanel {
            id: navigationPanel
            Layout.fillWidth: true

            onGoHome: content.source = "qrc:/HomeView.qml"
            onGoStore: browseStoreInteractor.displayDepartaments()
            onGoSettings: content.source = "qrc:/SettingsView.qml"
            onStoreQueryTyped: content.source = "qrc:/SearchView.qml"
        }

        Loader {
            id: content
            Layout.fillWidth: true
            Layout.fillHeight: true
            Layout.preferredHeight: 400

            source: "qrc:/HomeView.qml"
        }

        StatusArea {
            id: statusArea
            Layout.maximumHeight: statusArea.visible ? 38 : 0
            Layout.preferredHeight: statusArea.visible ? 38 : 0
            Layout.fillWidth: true
            Layout.bottomMargin: 4

            onVisibleChanged: print(visible)
        }
    }

    Interactors.DisableSnapInteractor {
        id: disableSnapInteractor
    }

    Interactors.EnableSnapInteractor {
        id: enableSnapInteractor
    }

    Interactors.RefreshSnapInteractor {
        id: refreshSnapInteractor
    }

    Interactors.RemoveSnapInteractor {
        id: removeSnapInteractor
    }

    Interactors.InstallSnapInteractor {
        id: installSnapInteractor
    }

    Interactors.BrowseStoreInteractor {
        id: browseStoreInteractor
        contentLoader: content
    }
}
