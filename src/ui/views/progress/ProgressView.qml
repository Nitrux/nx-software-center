import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.kde.mauikit 1.0 as Maui
import NXModels 1.0 as NX
import "../../templates"

Maui.Page
{
    id: control

    property bool isActive: true
    property alias manager : _progressManager


    NX.ProgressManager
    {
        id: _progressManager
        onWarning: notify("package-x-generic", "Oops", message)
    }

    Maui.Holder
    {
        body: qsTr("Downloads in progress will appear here")
        visible: _listView.count === 0
        emoji: "qrc:/download.svg"
        emojiSize: Maui.Style.iconSizes.huge
    }

    Maui.ListBrowser
    {
        id: _listView
        anchors.fill: parent
        orientation: ListView.Vertical
        spacing: Maui.Style.space.medium
        model: _progressManager

        delegate: PackageDelegate
        {
            id: _delegate
            width: ListView.view.width
        }
    }
}

