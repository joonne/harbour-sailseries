import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: seriesViewPage

    property string seriesID

    function initialize() {

        episodes.seriesID = seriesID

    }

    Component.onCompleted: initialize()

    Component.onDestruction: controller.SeriesListModel.Mode = "default"

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: "Remove"
                onClicked: {
                    controller.SeriesListModel.deleteSeries(seriesID)
                    pageStack.pop()
                }
            }

            MenuItem {
                text: "Update"
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
                Episodes { id: episodes }
                Seasons { id: seasons }
            }
        }
    }

    BusyIndicator {
           anchors.centerIn: parent
           running: controller.SeriesListModel.Loading ? true : false
       }
}
