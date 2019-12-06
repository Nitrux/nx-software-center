import QtQuick 2.10
import QtQuick.Controls 2.5
import QtQuick.Layouts 1.3
import org.kde.kirigami 2.7 as Kirigami
import org.kde.mauikit 1.0 as Maui

ColumnLayout
{
    id: control


    Maui.ListItemTemplate
    {
        Layout.fillWidth: true
        Layout.fillHeight: true

        label1.text: model.item.info.name
        label2.text: model.item.info.description
        label3.text: model.count
        imageSource: model.item.images[0].pic
    }

    RowLayout
    {
        Layout.fillWidth: true

        Column
        {
            Layout.fillWidth: true

            Label
            {
                text: model.item.package.name
                Layout.fillWidth: true
            }

            ProgressBar
            {
                Layout.fillWidth: true
                anchors.verticalCenter: parent.verticalCenter
            }
        }

        ToolButton
        {
            icon.name: "process-stop"
        }

        ToolButton
        {
            icon.name: "media-playback-start"
        }
    }


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
