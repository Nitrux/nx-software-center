import QtQuick 2.7
import QtQuick.Layouts 1.3

import org.kde.plasma.core 2.0 as PlasmaCore


RowLayout {
    spacing: 0
    
    property int rating : 0;

    Layout.rowSpan: 3
    Layout.alignment: Qt.AlignTop
    
    Repeater {
        model:rating
        delegate: PlasmaCore.IconItem {
            source: "starred-symbolic"
        }
    }
    
    Repeater {
        model: 5 - rating
        delegate: PlasmaCore.IconItem {
            Layout.alignment: Qt.AlignTop
            source: "non-starred-symbolic"
        }
    }
}
