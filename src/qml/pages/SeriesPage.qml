import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components"

Page {
    id: seriespage

    property string seriesID

    Component.onCompleted: {
        timer.start()
    }

    Component.onDestruction: {
        if (engine) {
            engine.SeriesListModel.Mode = "default"
        }
    }

    Timer {
        id: timer
        interval: 500
        onTriggered: {
            pageStack.pushAttached(Qt.resolvedUrl("SeasonsPage.qml"))
        }
    }

    function setStatus(status) {
        switch(status) {
            case "Continuing":
                return qsTr("Continuing")
            case "Ended":
                return qsTr("Ended")
            default:
                 return ""
        }
    }

    // add spaces to the given string after columns
    // "asd,asd,asd" => "asd, asd, asd"
    function process(string) { return string.split(",").join(", "); }

    SilicaFlickable {
        id: listView
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            MenuItem {
                text: qsTr("Remove")
                onClicked: {
                    remorse.execute(qsTr("Removing"),
                                    function() {
                                        engine.SeriesListModel.deleteSeries(seriesID);
                                        pageStack.pop()
                                    });
                }
            }

            MenuItem {
                text:qsTr("Update")
                onClicked: {
                    engine.SeriesListModel.updateSeries(seriesID);
                }
            }
        }

        Column {
            id: column
            width: parent.width
            spacing: Theme.paddingSmall

            PageHeader {
                id: header
                title: engine.SeriesListModel.SeriesName
            }

            SeriesBanner {
                id: banner
                bannerPath: engine.SeriesListModel.Banner
                sourceWidth: seriespage.width - Theme.paddingMedium * 2
            }

            Row {

                TextField {
                    id: status
                    label: qsTr("Status")
                    width: seriespage.width / 2
                    text: setStatus(engine.SeriesListModel.Status)
                    color: Theme.secondaryColor
                    readOnly: true
                }

                TextField {
                    id: rating
                    label: qsTr("Rating")
                    width: seriespage.width / 2
                    text: parseFloat(engine.SeriesListModel.Rating).toFixed(1)
                    color: Theme.secondaryColor
                    readOnly: true
                }

            }

            TextField {
                id: genre
                label: qsTr("Genre")
                width: seriespage.width
                text: process(engine.SeriesListModel.Genre)
                color: Theme.secondaryColor
                readOnly: true
            }

            TextExpander {
                id: expander
                width: seriespage.width
                textContent: engine.SeriesListModel.Overview
            }

            Button {
                id: imdb
                text: "IMDB"
                onClicked: Qt.openUrlExternally("http://www.imdb.com/title/" + engine.SeriesListModel.IMDB_ID)
                anchors.left: parent.left
                anchors.leftMargin: (seriespage.width - imdb.width) / 2
            }
        }
    }

    BusyIndicator {
        size: BusyIndicatorSize.Large
        anchors.centerIn: parent
        running: engine.SeriesListModel.Loading
    }

    RemorsePopup { id: remorse }
}
