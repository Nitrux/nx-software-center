import QtQuick 2.7
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.kde.mauikit 1.0 as Maui
import QtGraphicalEffects 1.0

import "../../templates"

StackView
{
    id: control

    Component
    {
        id: _appPageComponent

        AppPage
        {
            onExit: control.pop()
            buttonActions: [

                Button
                {
                    text: qsTr("Download")
                    Kirigami.Theme.textColor: "#37474F"
                    Kirigami.Theme.backgroundColor: Qt.rgba("#546E7A".r, "#546E7A".g, "#546E7A".b, 0.2)

                },

                Button
                {
                    text: qsTr("Run")
                    Kirigami.Theme.textColor: "#37474F"
                    Kirigami.Theme.backgroundColor: Qt.rgba("#546E7A".r, "#546E7A".g, "#546E7A".b, 0.2)

                },

                Button
                {
                    text: qsTr("Update")
                    Kirigami.Theme.textColor: Kirigami.Theme.positiveTextColor
                    Kirigami.Theme.backgroundColor: Qt.rgba(Kirigami.Theme.positiveTextColor.r, Kirigami.Theme.positiveTextColor.g, Kirigami.Theme.positiveTextColor.b, 0.2)

                },

                Button
                {
                    text: qsTr("Remove")
                    Kirigami.Theme.textColor: Kirigami.Theme.negativeTextColor
                    Kirigami.Theme.backgroundColor: Qt.rgba(Kirigami.Theme.negativeTextColor.r, Kirigami.Theme.negativeTextColor.g, Kirigami.Theme.negativeTextColor.b, 0.2)

                }
            ]
        }
    }

    initialItem: Maui.Page
    {
        padding: control.depth === 2 ? 0 : Maui.Style.space.big

        headBar.middleContent: [
            Maui.PathBar
            {
                Layout.fillWidth: true
                Layout.leftMargin: space.big
                Layout.rightMargin: space.big
                url: "Development/IDE/Sub"

                Maui.TextField
                {
                    visible: false
                }
            }
        ]

        headBar.rightContent: ToolButton
        {
            icon.name: "view-sort"
        }


        ListView
        {
            anchors.fill: parent

            Layout.fillWidth: true
            Layout.fillHeight: true
            orientation: ListView.Vertical
            spacing: Maui.Style.space.medium

//            headerPositioning: ListView.PullBackFooter
            header: Item
            {
                height: 250
                width: parent.width

                Rectangle
                {
                    id: _featuredSection
                    height: 200
                    width: parent.width
                    anchors.centerIn: parent
                    Kirigami.Theme.colorSet: Kirigami.Theme.Button
                    Kirigami.Theme.inherit: false
                    color: Kirigami.Theme.backgroundColor
                    border.color: Qt.tint(Kirigami.Theme.textColor, Qt.rgba(Kirigami.Theme.backgroundColor.r, Kirigami.Theme.backgroundColor.g, Kirigami.Theme.backgroundColor.b, 0.7))
                    radius: radiusV


                    ListView
                    {
                        id: _featuredListview
                        anchors.fill: parent
                        orientation: ListView.Horizontal
                        snapMode: ListView.SnapOneItem
                        clip: true

                        onMovementEnded: currentIndex = indexAt(contentX, contentY)


                        Maui.Badge
                        {
                            text: _featuredListview.currentIndex + 1
                            anchors.horizontalCenter: parent.horizontalCenter
                            z: 999
                            anchors.bottom: parent.bottom
                            anchors.margins: Maui.Style.space.big
                        }

                        model: ListModel
                        {
                            ListElement {name:"Index"; author:"Camilo Higuita"; organization:"Maui"; version: "1.0.0";  iconName: "index"; bannerImg: "qrc:/tests/banner_index.png";  count: 72000; itemInfo: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper."}
                            ListElement {name:"Vvave"; version: "1.0.0";  iconName: "vvave"; bannerImg: "qrc:/tests/banner_index.png";  count: 72000; itemInfo: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper."}
                        }

                        delegate: RowLayout
                        {
                            spacing: 0
                            height: _featuredListview.height
                            width: _featuredListview.width

                            Item
                            {
                                Layout.fillHeight: true
                                Layout.preferredWidth: parent.height
                                Layout.margins: 1
                                Layout.alignment: Qt.AlignCenter

                                Image
                                {
                                    id: _bannerImage
                                    height: parent.height
                                    width: parent.width
                                    sourceSize.height: height
                                    sourceSize.width: width
                                    source: model.bannerImg
                                    fillMode: Image.PreserveAspectCrop
                                    antialiasing: true
                                    smooth: true
                                    asynchronous: true

                                }

                                FastBlur
                                {
                                    id: fastBlur
                                    anchors.fill: parent
                                    source: _bannerImage
                                    radius: 50
                                    transparentBorder: false
                                    cached: true
                                }

                                Rectangle
                                {
                                    anchors.fill: parent
                                    color: Kirigami.Theme.viewBackgroundColor
                                    opacity: 0.7
                                }

                                Kirigami.Icon
                                {
                                    height: Kirigami.Units.iconSizes.huge
                                    width: height
                                    anchors.centerIn: parent
                                    source: model.iconName
                                }
                            }

                            Kirigami.Separator
                            {
                                Layout.fillHeight: true
                                Layout.preferredWidth: 1
                            }

                            Item
                            {
                                Layout.fillHeight: true
                                Layout.fillWidth: true
                                Layout.margins: Maui.Style.space.huge

                                ColumnLayout
                                {
                                    anchors.fill: parent
                                    spacing: 0
                                    Label
                                    {
                                        text: model.name
                                        visible: text.length
                                        Layout.fillWidth: true
                                        font.bold: true
                                        font.weight: Font.Bold
                                        elide: Text.ElideMiddle
                                        color: Kirigami.Theme.textColor
                                    }

                                    Label
                                    {
                                        text: model.author
                                        visible: text.length
                                        Layout.fillWidth: true
                                        font.weight: Font.Light
                                        elide: Text.ElideMiddle
                                        color: Kirigami.Theme.textColor
                                    }

                                    Label
                                    {
                                        text: model.organization
                                        visible: text.length
                                        Layout.fillWidth: true
                                        font.weight: Font.Light
                                        elide: Text.ElideMiddle
                                        color: Kirigami.Theme.textColor
                                    }

                                    Label
                                    {
                                        text: model.itemInfo
                                        visible: text.length
                                        Layout.topMargin: Maui.Style.space.big
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true
                                        font.pointSize: fontSizes.small
                                        font.weight: Font.Light
                                        wrapMode: Text.WrapAtWordBoundaryOrAnywhere
                                        elide: Text.ElideRight
                                        color: Kirigami.Theme.textColor
                                    }
                                }
                            }
                        }
                    }


                }

            }


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
                id: _delegate
                height: 100
                width: control.width - Maui.Style.space.huge
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

                Connections
                {
                    target: _delegate
                    onClicked:
                    {
                        control.push(_appPageComponent)

                        //for testing the model this sia custom info model
                        var appInfo = {
                            name: "Index",
                            version: "1.0.0",
                            author: "Camilo Higuita",
                            organization: "Maui",
                            bannerImage: "qrc:/tests/banner_index.png",
                            iconName: "index",
                            iconImage: "",
                            downloadsToday: 34,
                            updated: Date(),
                            license: "GPL v3",
                            itemInfo: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper. Curabitur fringilla rutrum mauris, in imperdiet dui commodo vitae. Fusce at enim ullamcorper, tincidunt orci a, tincidunt urna. Vivamus quis est ut elit dignissim semper. Vivamus erat leo, iaculis id faucibus ac, placerat eu lacus. Praesent arcu tortor, aliquet at volutpat eu, rutrum eget ante. Vivamus sed lorem sed massa interdum convallis. Suspendisse lacinia augue odio, id auctor ipsum varius id. Nam sit amet rhoncus augue. Vestibulum at lobortis velit. Proin ipsum eros, vehicula ut tempus vel, aliquam a orci. Fusce facilisis pulvinar orci. ",
                            changelogInfo: "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Integer eleifend est lectus, quis fringilla nisl luctus eu. Mauris a varius massa, sit amet venenatis massa. Nunc a ex ac urna aliquam egestas vitae ut est. Curabitur volutpat id turpis sed ullamcorper. Curabitur fringilla rutrum mauris, in imperdiet dui commodo vitae. Fusce at enim ullamcorper, tincidunt orci a, tincidunt urna. Vivamus quis est ut elit dignissim semper. Vivamus erat leo, iaculis id faucibus ac, placerat eu lacus. Praesent arcu tortor, aliquet at volutpat eu, rutrum eget ante. Vivamus sed lorem sed massa interdum convallis. Suspendisse lacinia augue odio, id auctor ipsum varius id. Nam sit amet rhoncus augue. Vestibulum at lobortis velit. Proin ipsum eros, vehicula ut tempus vel, aliquam a orci. Fusce facilisis pulvinar orci. ",
                            changelogVersion: "2.5.0"

                        }
                        control.currentItem.appInfo = appInfo
                    }
                }

            }
        }



    }
}
