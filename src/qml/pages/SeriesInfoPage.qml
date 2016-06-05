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
                visible: engine.SearchModel.Added ? enabled = false : enabled = true
                text: qsTr("Add to my series")
                onClicked: {
                    engine.SearchModel.getFullSeriesRecord(engine.SearchModel.ID)
                }
            }

            MenuItem {
                visible: engine.SearchModel.Added ? enabled = true : enabled = false
                text: qsTr("Already added")
                // TODO: maybe app could go to MySeriesPage ?
            }
        }

        Column {
            id: column
            spacing: Theme.paddingSmall

            PageHeader {
                id: header
                title: engine.SearchModel.SeriesName
            }

            Image {
                id: banner
                source: "http://thetvdb.com/banners/" + engine.SearchModel.Banner
                sourceSize.width: infopage.width - Theme.paddingMedium * 2
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingMedium

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

            Button {
                id: imdb
                text: "IMDB"
                onClicked: Qt.openUrlExternally("http://www.imdb.com/title/" + engine.SearchModel.IMDB_ID)
                anchors.left: parent.left
                anchors.leftMargin: (infopage.width - imdb.width) / 2
            }
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        running: engine.SearchModel.Loading ? true : false
    }
}
