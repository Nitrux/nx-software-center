import QtQuick 2.0
import QtQuick.Layouts 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

Item {
    id: root
    property variant contextIcon
    property string contextMessage
    property variant contextActions: []

    property variant dislpayActionsModel

    property bool locked: false
    property bool jobRunning: false
    property var currentJob: undefined

    function clearContext() {
        updateContext("", "", [])
    }

    function updateContext(icon, message, actions) {
        contextIcon = icon
        contextMessage = message
        contextActions = actions

//        print(message, locked)
        if (!locked)
            _updateView("notice", icon, message, actions)
    }

    function _updateView(urgency, icon, message, actions) {
        if (message.length == 0 && contextActions.length == 0)
            root.visible = false
        else
            root.visible = true

        if (urgency == "error")
            displayBackground.color = "red"
        else
            displayBackground.color = "transparent"

        displayIcon.source = icon
        displayLabel.text = message
        dislpayActionsModel = actions
    }

    function _onActionUpdate(message, actions) {
        root.jobRunning = true
        _updateView("notice", "documentinfo", message, actions)
    }

    function dissmiss() {
        jobRunning = false
        locked = false

        updateContext("", "", [])
    }
    function notice(message) {
        _updateView("notice", "notice", message, [])
    }

    function noticeError(message) {
        var dismisAction = {
            icon: "dialog-ok",
            text: i18n("Ok"),
            action: function () {
                locked = false
                _updateView("notice", contextIcon, contextMessage,
                            contextActions)
            }
        }

        _updateView("error", "error", message, [dismisAction])
    }
    function noticeSuccess(message) {
        var dismisAction = {
            icon: "dialog-ok",
            text: i18n("Ok"),
            action: function () {
                locked = false
                noticeDislpayTimer.stop()
                _updateView("notice", contextIcon, contextMessage,
                            contextActions)
            }
        }

        noticeDislpayTimer.start()
        _updateView("notice", "gtk-ok", message, [dismisAction])
    }
    function _onActionSuccess(message, actions) {
        var dismisAction = {
            icon: "dialog-ok",
            text: i18n("Ok"),
            action: function () {
                locked = false
                console.log("Success notification dismissed")
                noticeDislpayTimer.stop()
                _updateView("notice", contextIcon, contextMessage,
                            contextActions)
            }
        }

        noticeDislpayTimer.start()
        _updateView("notice", "gtk-ok", message, [dismisAction])
    }

    function _onActionError(message, actions) {
        _updateView("error", "error", message, actions)
        noticeDislpayTimer.start()
    }

    Timer {
        id: noticeDislpayTimer
        interval: 3000
        onTriggered: {
            locked = false
            _updateView("notice", contextIcon, contextMessage, contextActions)
        }
    }

    Rectangle {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top

        height: 1
        color: "lightgray"
    }

    Rectangle {
        id: displayBackground
        color: "green"
        anchors.fill: parent
        opacity: 0.3
    }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 6

        Item {
            Layout.leftMargin: 12
            Layout.preferredWidth: 24
            Layout.preferredHeight: 24

            PlasmaCore.IconItem {
                id: displayIcon
                anchors.fill: parent
                Layout.preferredHeight: 24

                visible: !root.jobRunning
            }
            PlasmaComponents.BusyIndicator {
                visible: root.jobRunning
                anchors.fill: parent
            }
        }

        PlasmaComponents.Label {
            id: displayLabel
            Layout.fillWidth: true
        }

        PlasmaComponents.ButtonRow {
            id: displayActions
            Layout.rightMargin: 18
            exclusive: false
            Layout.alignment: Qt.AlignRight | Qt.AlignVCenter

            Repeater {
                model: dislpayActionsModel
                delegate: PlasmaComponents.Button {
                    iconSource: dislpayActionsModel[index].icon
                    text: dislpayActionsModel[index].text
                    onClicked: {
                        dislpayActionsModel[index].action(_onActionUpdate,
                                                          _onActionSuccess,
                                                          _onActionError)
                    }
                }
            }
        }
    }
}
