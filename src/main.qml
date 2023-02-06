import QtQuick 2.7
import QtQuick.Controls 2.15

import org.mauikit.controls 1.3 as Maui

import NXModels 1.0 as NX

import "ui/views/apps"
import "ui/views/store"
import "ui/views/progress"

Maui.ApplicationWindow
{
    id: root
    readonly property var views: ({store: 0, apps: 1, progress: 2})

    width: 1000
    height: 800


    Maui.AppViews
    {
        id: _swipeView
        anchors.fill: parent
        showCSDControls: true
        altHeader: Maui.Handy.isMobile
        headBar.leftContent: Maui.ToolButtonMenu
         {
             icon.name: "application-menu"
             MenuItem
             {
                 text: i18n("Settings")
                 icon.name: "settings-configure"
             }

             MenuItem
             {
                 text: i18n("About")
                 icon.name: "documentinfo"
                 onTriggered: root.about()
             }
         }


        Maui.AppViewLoader
        {
            id: _storeViewLoader
            Maui.AppView.iconName: "nx-software-center"
            Maui.AppView.title: i18n("Store")

            property string pendingQuery
            StoreView
            {
                id: _storeView
                Component.onCompleted:
                {
                    if(_storeViewLoader.pendingQuery.length > 0)
                    {
                        _storeView.searchFor(pendingQuery)
                        pendingQuery = ""
                    }
                }
            }
        }

        Maui.AppViewLoader
        {
            Maui.AppView.iconName: "appimage-store"
            Maui.AppView.title: i18n("Apps")

            AppsView {}
        }

        Maui.AppViewLoader
        {
            Maui.AppView.iconName: "document-download"
            Maui.AppView.title: i18n("Progress")

            ProgressView {}
        }
    }

    function openView(view : String)
    {
        console.log("ASK ME TO OPAN A VIEW FORM QML");
        switch(view)
        {
        case "store": _swipeView.currentIndex = root.views.store; break;
        case "apps": _swipeView.currentIndex = root.views.apps; break;
        case "progress": _swipeView.currentIndex = root.views.progress; break;
        }
    }

    function openApp(appId: String)
    {
        _swipeView.currentIndex = root.views.store
    }

    function openCategory(category : String)
    {
        _swipeView.currentIndex = root.views.store

    }

    function searchFor(query : String)
    {
        if(_storeViewLoader.item)
        {
            _storeViewLoader.item.searchFor(query)
            _swipeView.currentIndex = root.views.store

        }else
        {
            _swipeView.currentIndex = root.views.store
            _storeViewLoader.pendingQuery = query
        }

    }
}
