import QtQuick 2.0
import QtGraphicalEffects 1.0
import QtQuick.Layouts 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents
import org.kde.plasma.extras 2.0 as PlasmaExtras

Item {
    height: innerLayout.implicitHeight
    width: innerLayout.implicitWidth

    property alias icon: iconItem.source
    property alias text: heading.text

    signal closeRequest()

    PlasmaCore.FrameSvgItem {
        imagePath: "opaque/widgets/panel-background"
        enabledBorders: PlasmaCore.FrameSvgItem.BottomBorder | PlasmaCore.FrameSvgItem.RightBorder
                        | PlasmaCore.FrameSvgItem.LeftBorder

        id: background
        anchors.fill: parent
    }

    RowLayout {
        id: innerLayout

        PlasmaCore.IconItem {
            id: iconItem
            Layout.margins: 4
            Layout.leftMargin: 8
            source: "dialog-information"
            implicitHeight: 32
        }

        PlasmaExtras.Heading {
            id: heading
            Layout.margins: 4
            Layout.minimumWidth: 300
            Layout.maximumWidth: 600
            wrapMode: Text.WordWrap

            level: 4
        }

        PlasmaComponents.ToolButton {
            Layout.margins: 4
            flat: true
            iconSource: "dialog-close"

            onClicked: closeRequest()
        }
    }
}
