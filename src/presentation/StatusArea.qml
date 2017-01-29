import QtQuick 2.0
import QtQuick.Layouts 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

Item {
    property variant contextIcon: "documentinfo"
    property string contextMessage: "Available actions"
    property variant contextActions: [{
            icon: "remove",
            text: "Sample broken action",
            action: function (update, success, error) {
                console.log("Sample broken action triggered")

                error("The actionn is broken :'(")
            }
        }, {
            icon: "games-solve",
            text: "Sample good action",
            action: function (update, success, error) {
                console.log("Sample good action triggered")
                update("Sample good action running")
                success("Sample good action finished")
            }
        }]

    property variant dislpayActionsModel

    property bool busy: false

    function updateContext(icon, message, actions) {
        contextIcon = icon
        contextMessage = message
        contextActions = actions

        if (!busy)
            _updateView("notice", icon, message, actions)
    }

    function _updateView(urgency, icon, message, actions) {
        if (urgency == "error")
            displayBackground.color = "red"
        else
            displayBackground.color = "transparent"

        displayIcon.source = icon
        displayLabel.text = message
        dislpayActionsModel = actions
    }

    function _onActionUpdate(message, actions) {
        _updateView("notice", "documentinfo", message, actions)
    }

    function _onActionSuccess(message, actions) {
        var dismisAction = {
            icon: "dialog-ok",
            text: i18n("Ok"),
            action: function () {
                console.log("Success notification dismissed")
                noticeDislpayTimer.stop()
                 _updateView("notice", contextIcon, contextMessage, contextActions)
            }
        }

        _updateView("notice", "gtk-ok", message, [dismisAction])
        noticeDislpayTimer.start()
    }

    function _onActionError(message, actions) {
        _updateView("error", "error", message, actions)
        noticeDislpayTimer.start()
    }


    Timer {
        id: noticeDislpayTimer
        interval: 3000
        onTriggered: _updateView("notice", contextIcon, contextMessage, contextActions)
    }

    Rectangle {
        id: displayBackground
        color: "green"
        anchors.fill: parent
        opacity: 0.3
    }

    RowLayout {
        anchors.fill: parent

        PlasmaCore.IconItem {
            id: displayIcon
            Layout.preferredHeight: 24
        }

        PlasmaComponents.Label {
            id: displayLabel
            Layout.fillWidth: true
        }

        PlasmaComponents.ButtonRow {
            id: displayActions
            exclusive: false
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

            Repeater {
                model: dislpayActionsModel
                delegate: PlasmaComponents.Button {
                    iconSource: dislpayActionsModel[index].icon
                    text: dislpayActionsModel[index].text
                    onClicked: dislpayActionsModel[index].action(_onActionUpdate, _onActionSuccess, _onActionError)
                }
            }
        }

        Component.onCompleted: _updateView("notice", contextIcon, contextMessage, contextActions)
    }
}
