import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.kde.mauikit 1.0 as Maui

import "../../templates"

Maui.Page
{
    id: control
    padding: Maui.Style.space.big

    headBar.rightContent: Kirigami.ActionToolBar
    {
        Layout.fillHeight: true
        Layout.fillWidth: true

        actions: [
        Action
            {
                icon.name: "document-share"
                text: qsTr("Sort")
            }

        ]
    }

    ListView
    {
        anchors.fill: parent
        orientation: ListView.Vertical
        spacing: Maui.Style.space.medium
        model: ListModel
        {
            ListElement{ category: "Applications"; label: "All"; icon: "appimage-store"; count: 72000; description: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper."}
            ListElement{ category: "Applications"; label: "Education"; icon: "applications-education"; count: 650; description: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper."}
            ListElement{ category: "Applications"; label: "Development"; icon: "applications-development"; count: 300; description: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper."}
            ListElement{ category: "Applications"; label: "Graphics"; icon: "applications-graphics"; count: 13}
            ListElement{ category: "Applications"; label: "Internet"; icon: "applications-internet"; description: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper."}
            ListElement{ category: "Applications"; label: "Games"; icon: "applications-games"; count: 7}
            ListElement{ category: "Applications"; label: "Multimedia"; icon: "applications-multimedia"; description: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper."}
            ListElement{ category: "Applications"; label: "Office"; icon: "applications-office"; description: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper."}
        }

        delegate: CardDelegate
        {
            //            showMenuIcon: true
            height: 100
            width: control.width * 0.9
            anchors.horizontalCenter: parent.horizontalCenter
            label1.text: model.label
            label2.text: model.description
            label3.text: model.count
            iconImg.source: model.icon

            quickButtons: [
                ToolButton
                {
                    icon.name: "document-share"
                },

                ToolButton
                {
                    icon.name: "media-playback-start"
                },

                ToolButton
                {
                    icon.name: "entry-delete"
                }
            ]

        }
    }

}
