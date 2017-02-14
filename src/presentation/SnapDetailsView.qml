import QtQuick 2.7
import QtQuick.Controls 1.4

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents


import org.nx.softwarecenter 1.0

ApplicationWindow {
    width: 800
    height: 600
    property string package_name;
    property var details;


    visible: true

    Component.onCompleted: {
        var request = SnapStore.getSnapDetails("sudo");
        request.complete.connect(function () {
            details = request.snapDetails();
            for (var k in details) {
                print(k, details[k])
            }
        })

        request.runAsync()
    }



}
