import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    anchors.top: parent.top
    height: seriesViewPage.height;
    width: seriesView.width;

    SilicaFlickable {
        contentHeight: column.height

        Column {
            // TODO get the page scrolling
            id: column

            PageHeader {
                id: header
                title: controller.SeriesListModel.SeriesName
            }

            Image {
                id: banner
                source: "http://thetvdb.com/banners/" + controller.SeriesListModel.Banner
                sourceSize.width: seriesViewPage.width - Theme.paddingMedium * 2
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingMedium
            }

            Row {

                TextField {
                    id: status
                    label: "Status"
                    width: seriesViewPage.width / 2
                    text: controller.SeriesListModel.Status
                    color: Theme.secondaryColor
                    readOnly: true
                }

                TextField {
                    id: rating
                    label: "Rating"
                    width: seriesViewPage.width / 2
                    text: controller.SeriesListModel.Rating
                    color: Theme.secondaryColor
                    readOnly: true
                }

            }

            TextArea {
                id: overview
                label: "Overview"
                width: seriesViewPage.width
                height: seriesViewPage.height * 0.4
                text: controller.SeriesListModel.Overview
                readOnly: true
                color: Theme.secondaryColor
            }

            BackgroundItem {
                TextField {
                    id: imdb
                    label: "IMDB"
                    text: "Show at IMDB"
                    onClicked: Qt.openUrlExternally("http://www.imdb.com/title/" + controller.SeriesListModel.IMDB_ID)
                    readOnly: true
                    color: Theme.secondaryColor
                }
            }
        }
    }
}
