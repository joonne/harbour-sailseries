import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components"

Page {
    id: seriespage

    property int seriesId

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

    function addSpaces(str) { return str.split(",").join(", ") }

    SilicaFlickable {
        id: listView
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            busy: engine.Loading

            MenuItem {
                text: qsTr("Remove")
                onClicked: {
                    remorse.execute(qsTr("Removing"),
                                    function() {
                                        engine.SeriesListModel.deleteSeries(seriesId);
                                        pageStack.pop()
                                    });
                }
            }

            MenuItem {
                text:qsTr("Update")
                onClicked: {
                    engine.SeriesListModel.updateSeries(seriesId);
                }
            }

            MenuItem {
                text: "IMDB"
                onClicked: Qt.openUrlExternally("http://www.imdb.com/title/" + engine.SeriesListModel.IMDB_ID)
            }
        }

        Column {
            id: column
            width: parent.width
            spacing: Theme.paddingSmall

            PageHeader {
                id: header
                title: engine.SeriesListModel.SeriesName
                description: engine.SeriesListModel.Network
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
                    readOnly: true
                }

                TextField {
                    id: rating
                    label: qsTr("Rating")
                    width: seriespage.width / 2
                    text: parseFloat(engine.SeriesListModel.Rating).toFixed(1)
                    readOnly: true
                }

            }

            TextField {
                id: genre
                label: qsTr("Genre")
                width: seriespage.width
                text: addSpaces(engine.SeriesListModel.Genre)
                readOnly: true
            }

            TextArea {
                label: qsTr("Overview")
                width: seriespage.width
                text:  engine.SeriesListModel.Overview
                readOnly: true
            }

            VerticalScrollDecorator {
                id: decorator
            }
        }
    }

    RemorsePopup { id: remorse }
}
