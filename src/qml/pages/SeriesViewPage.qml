import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailseries.datamodel 1.0

Page {
    id: seriesViewPage

    property string seriesID

    function initialize() {

        episodes.initialize(seriesID);

    }

    Component.onCompleted: initialize()

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            MenuItem {
                text: "Remove series"
                onClicked: {
                    console.log("poistetaan sarjaa " + seriesID)
                    DATAMODEL.SeriesListModel.deleteSeries(seriesID)
                    pageStack.pop()
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
}
