import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: seriesViewPage

    property string seriesID
    property string pageName: "SeriesViewPage"

    function updateModel() {
        seasons.setModel();
    }

    Component.onDestruction: controller.SeriesListModel.Mode = "default"

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: qsTr("Remove")
                onClicked: {
                    remorse.execute(qsTr("Removing"),
                                           function() {
                                               controller.SeriesListModel.deleteSeries(seriesID);
                                               pageStack.pop()
                                           });
                }
            }

            MenuItem {
                text:qsTr("Update")
                onClicked: {
                    controller.SeriesListModel.updateSeries(seriesID);
                }
            }
        }

        SlideshowView {

            id: seriesView
            width: seriesViewPage.width
            height: seriesViewPage.height
            itemWidth: width

            model: VisualItemModel {
                id: items
                Series { id: series }
                Seasons { id: seasons }
            }
        }
    }

    BusyIndicator {
        anchors.centerIn: parent
        running: controller.SeriesListModel.Loading ? true : false
    }

    RemorsePopup { id: remorse }
}
