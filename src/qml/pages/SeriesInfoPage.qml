import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: infopage

    Component.onCompleted: busyIndicator.running = false

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            MenuItem {
                visible: controller.SearchModel.Added ? enabled = false : enabled = true
                text: "Add to my series"
                onClicked: controller.SearchModel.getFullSeriesRecord(controller.SearchModel.ID)
            }

            MenuItem {
                visible: controller.SearchModel.Added ? enabled = true : enabled = false
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
                title: controller.SearchModel.SeriesName
            }

            Image {
                id: banner
                source: "http://thetvdb.com/banners/" + controller.SearchModel.Banner
                sourceSize.width: infopage.width - Theme.paddingMedium * 2
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingMedium

            }

            TextArea {
                id: overview
                label: "Overview"
                width: infopage.width
                text: controller.SearchModel.Overview
                readOnly: true
                color: Theme.secondaryColor
            }

            TextField {
                id: network
                label: "Network"
                text: controller.SearchModel.Network
                color: Theme.secondaryColor
                readOnly: true
                width: infopage.width
            }

            BackgroundItem {
                TextField {
                    id: imdb
                    label: "IMDB"
                    text: "Show at IMDB"
                    onClicked: Qt.openUrlExternally("http://www.imdb.com/title/" + controller.SearchModel.IMDB_ID)
                    readOnly: true
                    color: Theme.secondaryColor
                    width: infopage.width
                }
            }
        }
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
        running: controller.SearchModel.Loading ? true : false
       }
}
