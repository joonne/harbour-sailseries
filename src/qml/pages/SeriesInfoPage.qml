import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components"

Page {
    id: infopage

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            MenuItem {
                text: "IMDB"
                onClicked: Qt.openUrlExternally("http://www.imdb.com/title/" + engine.SearchModel.IMDB_ID)
            }

            MenuItem {
                visible: engine.SearchModel.Added === false
                text: qsTr("Add to my series")
                onClicked: {
                    engine.SearchModel.getFullSeriesRecord(engine.SearchModel.ID)
                }
            }

            MenuItem {
                enabled: engine.SearchModel.Added === false
                visible: engine.SearchModel.Added === true
                text: qsTr("Already added")
            }
        }

        Column {
            id: column
            spacing: Theme.paddingSmall

            PageHeader {
                id: header
                title: engine.SearchModel.SeriesName
            }

            SeriesBanner {
                id: banner
                bannerPath: engine.SearchModel.Banner
                sourceWidth: infopage.width - Theme.paddingMedium * 2
            }

            HorizontalSeparator { }

            TextExpander {
                id: expander
                width: infopage.width
                textContent: engine.SearchModel.Overview
            }

            TextField {
                id: network
                label: qsTr("Network")
                text: engine.SearchModel.Network
                color: Theme.secondaryColor
                readOnly: true
                width: infopage.width
            }
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        size: BusyIndicatorSize.Large
        running: engine.SearchModel.Loading ? true : false
    }
}
