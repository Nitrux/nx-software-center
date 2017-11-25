import QtQuick 2.0
import QtGraphicalEffects 1.0

import org.kde.plasma.core 2.0 as PlasmaCore

Item {
    PlasmaCore.FrameSvgItem {
        imagePath: "opaque/widgets/panel-background"
        enabledBorders: PlasmaCore.FrameSvgItem.BottomBorder | PlasmaCore.FrameSvgItem.RightBorder | PlasmaCore.FrameSvgItem.LeftBorder

        id: background
        width: parent.width
        height: parent.height
    }
}
