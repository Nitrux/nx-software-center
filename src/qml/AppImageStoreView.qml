import QtQuick 2.0

import org.nx.softwarecenter 1.0

import "parts" as Parts

Parts.View {
    id: appImageStoreViewRoot

    function query(text) {
        SearchViewController.search(text);
    }

    Component.onCompleted: {
        print(query())
        query("")
    }

    Connections {
        target: SearchViewController
        onApplications: appImageHubListView.model = apps
        onNoMatchFound: {
            appImageHubListView.model = undefined
        }
    }

    SnapGrid {
        id: appImageHubListView

        anchors.fill: parent

        delegate: GridViewItemDelegate {
            name: model.modelData['name']
            version: model.modelData['latest_release_id']
            size: model.modelData['download_size']
        }
    }

}
