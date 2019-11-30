import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components"

Page {
    id: seriespage

    property int seriesId
    property string imdbId
    property string seriesName
    property string network
    property string banner
    property string seriesStatus
    property string rating
    property string genre
    property string overview

    Component.onCompleted: {
        timer.start()
    }

    Component.onDestruction: {
        if (engine) {
            engine.Mode = "default"
        }
    }

    Timer {
        id: timer
        interval: 500
        onTriggered: {
            pageStack.pushAttached(Qt.resolvedUrl("SeasonsPage.qml"), { seriesId: seriesId })
        }
    }

    function getStatus(aStatus) {
        switch(aStatus) {
            case "Continuing":
                return qsTr("Continuing")
            case "Ended":
                return qsTr("Ended")
            default:
                return ""
        }
    }

    function formatRating(aRating) {
        return parseFloat(aRating).toFixed(1)
    }

    function formatGenre(aGenre) {
        return aGenre.split(",").join(", ")
    }

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
                                        engine.SeriesListModel.deleteSeries(seriesId)
                                        pageStack.pop()
                                    });
                }
            }

            MenuItem {
                text:qsTr("Update")
                onClicked: {
                    engine.SeriesListModel.updateSeries(seriesId)
                }
            }

            MenuItem {
                text: "IMDB"
                onClicked: Qt.openUrlExternally("http://www.imdb.com/title/" + imdbId)
            }
        }

        Column {
            id: column
            width: parent.width
            spacing: Theme.paddingSmall

            PageHeader {
                id: header
                title: seriesName
                description: network
            }

            SeriesBanner {
                bannerPath: banner
                sourceWidth: seriespage.width - Theme.paddingMedium * 2
            }

            Row {
                TextField {
                    label: qsTr("Status")
                    width: seriespage.width / 2
                    text: getStatus(seriesStatus)
                    readOnly: true
                }

                TextField {
                    label: qsTr("Rating")
                    width: seriespage.width / 2
                    text: formatRating(rating)
                    readOnly: true
                }

            }

            TextField {
                label: qsTr("Genre")
                width: seriespage.width
                text: formatGenre(genre)
                readOnly: true
            }

            TextArea {
                label: qsTr("Overview")
                width: seriespage.width
                text:  overview
                readOnly: true
            }

            VerticalScrollDecorator {
                id: decorator
            }
        }
    }

    RemorsePopup { id: remorse }
}
