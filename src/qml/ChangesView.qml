import QtQuick 2.0

import QtQuick.Layouts 1.0
import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import org.nx.softwarecenter 1.0 as SoftwareCenter

Item {
    Rectangle {
        color: "lightgreen"
        opacity: 0.3
        anchors.fill: parent
    }
    PlasmaComponents.Label {
        text: "Tasks"
        font.pointSize: 14
    }

    ColumnLayout {
        // padding: 12
        spacing: 24
        anchors.fill: parent

        Repeater {
            model: SoftwareCenter.Snaps.changesModel()
            delegate: GridLayout {
                columns: 2
                height: 40
                width: 300
                property var currentTask: tasks[tasks.length -1]
                onCurrentTaskChanged: {
                    print("Current task updated:\n", JSON.stringify(currentTask));
                }


                PlasmaCore.IconItem {
                    source: {
                        var kind_icon = {
                            "remove-snap" : "package-remove"
                        }
                        print (kind)
                        return kind_icon[kind];
                    }
                    height: 30;
                }

                PlasmaComponents.Label {
                    text: summary
                }

                PlasmaComponents.BusyIndicator {
                    visible: !ready
                    height: 24
                }

                PlasmaComponents.Label {
                    text: currentTask.summary
                    visible: !ready
                }

            }
        }
    }
}
