import QtQuick 2.0

import org.nx.softwarecenter 1.0

import "parts" as Parts

Parts.View {
    id: appImageStoreViewRoot

    QtObject {
        id: p
        property string query
    }

    function query( text ) {
        p.query = query
        refreshModel()
    }

    function refreshModel() {
        console.log("refreshing model " + p.query)
//        if (p.query)
            appImageHubListView.model = AppImageHubRepository.listAsVariant()
//        else
//            appImageHubListView.model = AppImageHubRepository.searchAsVariant(appImageStoreViewRoot.query)

    }

    Connections {
        target: AppImageHubRepository
        onItemsChanged: refreshModel()
    }

    SnapGrid {
        id: appImageHubListView

        anchors.fill: parent

        delegate: GridViewItemDelegate {
            name: model.modelData.id
        }
    }

}
