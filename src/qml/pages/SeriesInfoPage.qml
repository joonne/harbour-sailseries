import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailseries.datamodel 1.0

Page {
    id: infopage

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            MenuItem {
                visible: DATAMODEL.SearchModel.Added ? enabled = false : enabled = true
                text: "Add to my series"
                onClicked: DATAMODEL.SearchModel.getFullSeriesRecord(DATAMODEL.SearchModel.ID)
            }

            MenuItem {
                visible: DATAMODEL.SearchModel.Added ? enabled = true : enabled = false
                text: "Already added"
                // TODO: maybe app could go to MySeriesPage ?
            }
        }

        Column {
            id: column
            anchors.bottom: parent.bottom
            anchors.bottomMargin: Theme.paddingMedium

            PageHeader {
                id: header
                title: DATAMODEL.SearchModel.SeriesName
            }

            Image {
                id: banner
                source: "http://thetvdb.com/banners/" + DATAMODEL.SearchModel.Banner
                sourceSize.width: infopage.width - Theme.paddingMedium * 2
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingMedium

            }

            TextArea {
                id: overview
                label: "Overview"
                width: infopage.width
                text: DATAMODEL.SearchModel.Overview
                readOnly: true
                color: Theme.secondaryColor
            }

            TextField {
                id: network
                label: "Network"
                text: DATAMODEL.SearchModel.Network
                color: Theme.secondaryColor
                readOnly: true
                width: infopage.width
            }

            BackgroundItem {
                TextField {
                    id: imdb
                    label: "IMDB"
                    text: "Show at IMDB"
                    onClicked: Qt.openUrlExternally("http://www.imdb.com/title/" + DATAMODEL.SearchModel.IMDB_ID)
                    readOnly: true
                    color: Theme.secondaryColor
                    width: infopage.width
                }
            }
        }
    }

    BusyIndicator {
           anchors.centerIn: parent
           running: DATAMODEL.SearchModel.Loading ? true : false
       }
}
