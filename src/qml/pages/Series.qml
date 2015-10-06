import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components"

Item {
    anchors.top: parent.top
    height: seriesViewPage.height
    width: seriesView.width

    function setStatus(status) {
        if(status === "Continuing") {
            return qsTr("Continuing")
        } else if(status === "Ended") {
            return qsTr("Ended")
        }
    }

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
                    label: qsTr("Status")
                    width: seriesViewPage.width / 2
                    text: setStatus(controller.SeriesListModel.Status)
                    color: Theme.secondaryColor
                    readOnly: true
                }

                TextField {
                    id: rating
                    label: qsTr("Rating")
                    width: seriesViewPage.width / 2
                    text: controller.SeriesListModel.Rating
                    color: Theme.secondaryColor
                    readOnly: true
                }

            }

            TextArea {
                id: overview
                label: qsTr("Overview")
                width: seriesViewPage.width
                height: seriesViewPage.height * 0.4
                text: controller.SeriesListModel.Overview
                readOnly: true
                color: Theme.secondaryColor
            }

//            TextExpander {
//                id: expander
//                width: seriesView.width
//                textContent: controller.SeriesListModel.Overview
//            }

            Button {
                id: imdb
                text: "IMDB"
                onClicked: Qt.openUrlExternally("http://www.imdb.com/title/" + controller.SeriesListModel.IMDB_ID)
                anchors.left: parent.left
                anchors.leftMargin: (seriesView.width - imdb.width) / 2
            }
        }
    }
}
