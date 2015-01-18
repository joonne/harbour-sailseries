import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailseries.datamodel 1.0

Item {
    anchors.top: parent.top
    height: seriesViewPage.height;
    width: seriesView.width;

    Column {
        // TODO
        id: column

        PageHeader {
            id: header
            title: DATAMODEL.SeriesListModel.SeriesName
        }

        Image {
            id: banner
            source: "http://thetvdb.com/banners/" + DATAMODEL.SeriesListModel.Banner
            sourceSize.width: seriesViewPage.width - Theme.paddingMedium * 2
            anchors.left: parent.left
            anchors.leftMargin: Theme.paddingMedium
        }

        TextArea {
            id: overview
            label: "Overview"
            width: seriesViewPage.width
            text: DATAMODEL.SeriesListModel.Overview
            readOnly: true
            color: Theme.secondaryColor
        }

        TextField {
            id: network
            label: "Network"
            text: DATAMODEL.SeriesListModel.Network
            color: Theme.secondaryColor
            readOnly: true
        }

        BackgroundItem {
            TextField {
                id: imdb
                label: "IMDB"
                text: "Show at IMDB"
                onClicked: Qt.openUrlExternally("http://www.imdb.com/title/" + DATAMODEL.SeriesListModel.IMDB_ID)
                readOnly: true
                color: Theme.secondaryColor
            }
        }
    }

}
