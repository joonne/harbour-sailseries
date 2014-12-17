import QtQuick 2.0
import Sailfish.Silica 1.0
import DataModel 1.0

Page {
    id: infopage

    //property SeriesListModel listModel

//    Component.onCompleted: {
//        console.log("SeriesInfoPage on complete " + listModel);
//    }

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            MenuItem {
                text: "Add to my series"
                onClicked: LISTMODEL.getFullSeriesRecord(LISTMODEL.ID)
            }
        }

        Column {
            id: column

            PageHeader {
                id: header
                title: LISTMODEL.SeriesName
            }

            Separator {
                height: Theme.PaddingMedium * 3
                width: infopage.width - Theme.PaddingMedium * 2
                color: Theme.secondaryColor
            }

            Image {
                id: banner
                source: "http://thetvdb.com/banners/" + LISTMODEL.Banner
                sourceSize.width: infopage.width
            }


            Separator {
                height: Theme.PaddingMedium * 3
                width: infopage.width - Theme.PaddingMedium * 2
                color: Theme.secondaryColor
            }

            TextArea {
                id: overview
                label: "Overview"
                width: infopage.width
                text: LISTMODEL.Overview
                readOnly: true
                color: Theme.secondaryColor
            }


            Separator {
                height: Theme.PaddingMedium
                width: infopage.width - Theme.PaddingMedium * 2
                color: Theme.secondaryColor

            }

            TextField {
                id: network
                label: "Network"
                text: LISTMODEL.Network
                color: Theme.secondaryColor
                readOnly: true
            }

            BackgroundItem {
                TextField {
                    id: imdb
                    label: "IMDB"
                    text: "Show at IMDB"
                    onClicked: Qt.openUrlExternally("http://www.imdb.com/title/" + LISTMODEL.IMDB_ID)
                    readOnly: true
                    color: Theme.secondaryColor
                }
            }
        }
    }

    BusyIndicator {
           anchors.centerIn: parent
           running: LISTMODEL.Loading ? true : false
       }
}
