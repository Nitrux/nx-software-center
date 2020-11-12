import QtQuick 2.14
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.14 as Kirigami
import org.kde.mauikit 1.2 as Maui
import QtGraphicalEffects 1.0

import "../../templates"
import NXModels 1.0 as NX

StackView
{
    id: control

    Component
    {
        id: _appPageComponent

        AppPage
        {
            id: _appPage
            onExit: control.pop()

            onPackageClicked:
            {
                console.log("trying to append package app ", app.id)
                _progressView.manager.appendPackage(app, index, NX.Package.DOWNLOAD)
            }

            buttonActions: [

                Button
                {
                    text: qsTr("Download")
                    visible: !app.isInstalled
                    Kirigami.Theme.textColor: "#37474F"
                    Kirigami.Theme.backgroundColor: Qt.rgba("#546E7A".r, "#546E7A".g, "#546E7A".b, 0.2)
                    onClicked: {
                        _appPage.scrollTo(AppPage.Sections.Packages)
                    }
                },

                Button
                {
                    text: qsTr("Run")
                    visible: app.isInstalled
                    onClicked: app.launchApp()
                    Kirigami.Theme.textColor: "#37474F"
                    Kirigami.Theme.backgroundColor: Qt.rgba("#546E7A".r, "#546E7A".g, "#546E7A".b, 0.2)

                },

                Button
                {
                    text: qsTr("Update")
                    visible: app.isInstalled && app.isUpdatable
                    onClicked: app.updateApp()
                    Kirigami.Theme.textColor: Kirigami.Theme.positiveTextColor
                    Kirigami.Theme.backgroundColor: Qt.rgba(Kirigami.Theme.positiveTextColor.r, Kirigami.Theme.positiveTextColor.g, Kirigami.Theme.positiveTextColor.b, 0.2)

                },

                Button
                {
                    text: qsTr("Remove")
                    visible: app.isInstalled
                    onClicked: app.removeApp()
                    Kirigami.Theme.textColor: Kirigami.Theme.negativeTextColor
                    Kirigami.Theme.backgroundColor: Qt.rgba(Kirigami.Theme.negativeTextColor.r, Kirigami.Theme.negativeTextColor.g, Kirigami.Theme.negativeTextColor.b, 0.2)

                }
            ]
        }
    }

    initialItem: StoreFrontPage
    {
        id: _frontPage

        onItemClicked:
        {
            control.push(_appPageComponent)
            control.currentItem.data = app
        }
    }
}
