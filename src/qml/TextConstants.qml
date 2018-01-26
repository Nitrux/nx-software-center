import QtQuick 2.0

QtObject {
    readonly property string actionRefreshTitle: i18n("Re&fresh")
    readonly property string actionRemoveTitle: i18n("&Remove")
    readonly property string actionEnableTitle: i18n("&Enable")
    readonly property string actionDisableTitle: i18n("&Disable")
    readonly property string actionInstallTitle: i18n("&Install")
    readonly property string actionApplySettingsTitle: i18n("&Apply")
    readonly property string actionRetryTitle: i18n("R&etry")
    readonly property string actionOkTitle: i18n("&Ok")
    readonly property string actionCancelTitle: i18n("&Cancel")
    readonly property string unknownError: i18n("Ups! something went wrong.")
    readonly property string fetchError: i18n("Unable to fetch applications list, please check your internet connection.")
    readonly property string requestCredentialNotice: i18n("Requesting user credentials")
    readonly property string availableActionsNotice: i18n("Available actions")
    readonly property string noApplicationsFound: i18n("Sorry, no applications were found. Please try search something else.")

}

