import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components"

Item {
    id: seriesItem
    height: seriesView.height;
    width: seriesView.width;

    function setStatus(status) {
        if(status === "Continuing") {
            return qsTr("Continuing")
        } else if(status === "Ended") {
            return qsTr("Ended")
        }
    }

    function process(string) {

        if(string.charAt(0) === "|" && string.charAt(string.length - 1) === "|") {
            var newstring = string.split("|").join(", ")
            return newstring.substr(2,(newstring.length - 4))
        } else if(string.charAt(0) === "|") {
            return string.split("|").join(", ").substr(2)
        } else {
            return string.split("|").join(", ")
        }
    }

    SilicaListView {
        id: listView
        anchors.fill: parent
        model: 1

        delegate: ListItem {
            contentHeight: column.height + Theme.paddingMedium
            contentWidth: listView.width

            Column {
                id: column
                spacing: Theme.paddingSmall

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

                TextField {
                    id: genre
                    label: qsTr("Genre")
                    width: seriesViewPage.width
                    text: process(controller.SeriesListModel.Genre)
                    color: Theme.secondaryColor
                    truncationMode: TruncationMode.Fade
                    readOnly: true
                }

                TextExpander {
                    id: expander
                    width: seriesViewPage.width
                    textContent: controller.SeriesListModel.Overview
                }

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
}
