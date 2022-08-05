import QtQuick 2.14
import QtQuick.Controls 2.14

import org.mauikit.controls 1.3 as Maui

import NXModels 1.0 as NX
import org.maui.nxsc 1.0

import "../../templates"

StackView
{
    id: control

    property NX.Category currentCategory : _categoriesList.baseCategory()
    property var tags : []
    readonly property alias frontPage : _frontPage

    NX.Categories
    {
        id: _categoriesList
    }

    Component
    {
        id: _appPageComponent

        AppPage
        {
            id: _appPage
            onGoBackTriggered: control.pop()

            headBar.leftContent: ToolButton
            {
                icon.name: "go-previous"
                onClicked: control.pop()
                text: control.get(_appPage.StackView.index-1, StackView.DontLoad).title
                display: isWide ? ToolButton.TextBesideIcon : ToolButton.IconOnly
            }

            onTagClicked:
            {
                control.tags = [tag]
                control.currentCategory = _categoriesList.baseCategory()
                control.push(_categoryPageComponent)
            }

            onPackageClicked:
            {
                console.log("trying to append package app ", app.downloads[index].link)
                // _progressManager.appendPackage(app, index)
                InstallService.installFromPlingStore(app.downloads[index].link, app.data);
            }

            buttonActions: [

                Button
                {
                    text: qsTr("Get")
                    visible: !app.isInstalled
                    Maui.Theme.textColor: "#fafafa"
                    Maui.Theme.backgroundColor: "#26c6da"
                    Maui.Theme.hoverColor: "#26c6da"
                    Maui.Theme.highlightColor: "#26c6da"
                    onClicked:
                    {
                        _appPage.scrollTo(AppPage.Sections.Packages)
                    }
                },

                Button
                {
                    text: qsTr("Run")
                    visible: app.isInstalled
                    onClicked: app.launchApp()
                    Maui.Theme.textColor: "#37474F"
                    Maui.Theme.backgroundColor: Qt.rgba("#546E7A".r, "#546E7A".g, "#546E7A".b, 0.2)

                },

                Button
                {
                    text: qsTr("Update")
                    visible: app.isInstalled && app.isUpdatable
                    onClicked: app.updateApp()
                    Maui.Theme.textColor: Maui.Theme.positiveTextColor
                    Maui.Theme.backgroundColor: Qt.rgba(Maui.Theme.positiveTextColor.r, Maui.Theme.positiveTextColor.g, Maui.Theme.positiveTextColor.b, 0.2)

                },

                Button
                {
                    text: qsTr("Remove")
                    visible: app.isInstalled
                    onClicked: app.removeApp()
                    Maui.Theme.textColor: Maui.Theme.negativeTextColor
                    Maui.Theme.backgroundColor: Qt.rgba(Maui.Theme.negativeTextColor.r, Maui.Theme.negativeTextColor.g, Maui.Theme.negativeTextColor.b, 0.2)

                }
            ]
        }
    }

    Component
    {
        id: _categoryPageComponent

        StoreCategoryPage
        {
            id: _categoryPage
            showTitle: false
            category : currentCategory
            tags: control.tags
            onGoBackTriggered: control.pop()

            headBar.farLeftContent: ToolButton
            {
                icon.name: "go-previous"
                text: control.get(_categoryPage.StackView.index-1, StackView.DontLoad).title
                display: isWide ? ToolButton.TextBesideIcon : ToolButton.IconOnly
                onClicked: control.pop()
            }

            onItemClicked:
            {
                control.push(_appPageComponent)
                control.currentItem.data = app
            }
        }
    }

    initialItem: StoreFrontPage
    {
        id: _frontPage
//        title: i18n("Store")
//        showTitle: false

        onItemClicked:
        {
            control.push(_appPageComponent)
            control.currentItem.data = app
        }

        onCategoryClicked:
        {
            currentCategory = category
            control.tags = []
            control.push(_categoryPageComponent)
        }

        onSearchFor:
        {
            control.currentCategory = _categoriesList.baseCategory()
            control.tags = []
            control.push(_categoryPageComponent)
            control.currentItem.search(query)
        }
    }
}
