import QtQuick 2.14
import QtQuick.Controls 2.14

import org.kde.kirigami 2.14 as Kirigami
import org.mauikit.controls 1.3 as Maui

import NXModels 1.0 as NX

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
                console.log("trying to append package app ", app.id)
                _progressView.manager.appendPackage(app, index, NX.Package.DOWNLOAD)
            }

            buttonActions: [

                Button
                {
                    text: qsTr("Get")
                    visible: !app.isInstalled
                    Kirigami.Theme.textColor: "#fafafa"
                    Kirigami.Theme.backgroundColor: "#26c6da"
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
        title: i18n("Store")
        showTitle: false

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
            currentCategory = _categoriesList.baseCategory()
            control.tags = []
            control.push(_categoryPageComponent)
            control.currentItem.search(query)
        }
    }
}
