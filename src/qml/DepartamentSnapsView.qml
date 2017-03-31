import QtQuick 2.7
import QtQuick.Controls 1.4
import QtQuick.Controls.Styles 1.4
import QtQuick.Layouts 1.3
import QtGraphicalEffects 1.0

import org.kde.plasma.core 2.0 as PlasmaCore
import org.kde.plasma.components 2.0 as PlasmaComponents

import Snapd 1.0

import org.nx.softwarecenter 1.0
import "qrc:/scripts/Utils.js" as UtilsJs

import "parts" as Parts
import "interactors" as Interactors

Parts.View {
    id: departamentSnaps
    property string departamentSlug: ""

    function updateActions() {
        var keys = departamentSnapsModel.getSelectedItems()
        if (keys.length > 0) {
            var installAction = {
                icon: "package-install",
                text: textConstants.actionInstallTitle,
                action: function () {
                    var targets_names = departamentSnapsModel.getSelectedItems()
                    var targets = []
                    for (var i in targets_names) {
                        var model = departamentSnapsModel.getByName(
                                    targets_names[i])
                        targets.push({
                                         name: model.package_name,
                                         channel: model.channel
                                     })
                    }

                    installSnapInteractor.targets = targets
                    installSnapInteractor.finished.connect(updateActions)

                    installSnapInteractor.start()
                }
            }

            var actions = [installAction]
            statusArea.updateContext("emblem-info",
                                     textConstants.availableActionsNotice,
                                     actions)
        } else
            statusArea.clearContext()
    }

    Interactors.ListSnapsInDepartamentInteractor {
        id: listSnapsInDepartamentInteractor
        departament: departamentSnaps.departamentSlug
        onDepartamentChanged: {
            //            print(departament)
            run()
        }
        onLoading: showLoadingScreen(i18n("Loading snaps"))

        onFinished: {
            if (snaps.length == 0) {
                departamentSnaps.sourceComponent = component_noSnaps
            } else {
                departamentSnaps.sourceComponent = component_snapGrid
                depdocumentinfoartamentSnapsModel.clear()
                for (var i in snaps)
                    departamentSnapsModel.append(snaps[i])
            }
        }
        onError: showError(message)
    }

    Parts.SnapsModel {
        id: departamentSnapsModel

        property var snaps
        onSelectedItemsChanged: {
            updateActions()
        }

        function _indexOf(name) {
            return UtilsJs.binaryIndexOf(name, function (a, b) {
                return a.package_name.localeCompare(b)
            }, function () {
                return count
            }, function (i) {
                return get(i)
            })
        }
    }

    Component {
        id: component_snapGrid
        SnapGrid {
            model: departamentSnapsModel
            delegate: SnapElementDelegate {
                snap_name: title
                snap_version: version
                snap_size: model.downloadSize !== undefined ? downloadSize : -1
                // HACK: for some reason the icon_url property is not accesible at the model
                snap_icon: listSnapsInDepartamentInteractor.snaps[index]
                           && listSnapsInDepartamentInteractor.snaps[index].icon_url
                           !== undefined ? listSnapsInDepartamentInteractor.snaps[index].icon_url : ""
                onSelectedChanged: {
                    if (selected)
                        departamentSnapsModel.selectedItems[package_name] = "true"
                    else
                        delete departamentSnapsModel.selectedItems[package_name]

                    updateActions()
                }
                onClicked: main.showSnapDetails(package_name)
            }
        }
    }

    Component {
        id: component_noSnaps
        PlaceHolderView {
            message: textConstants.noSnapsFound
            iconName : "face-sad"
            showBusyIndicator : false
        }
    }
}
