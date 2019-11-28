import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components";

Page {
    id: myseriespage

    Component.onCompleted: {
        engine.SeriesListModel.populate()
    }

    Component.onDestruction: {
        if (engine) {
            engine.SeriesListModel.Mode = "default"
        }
    }

    function nextEpisodeDetails(episodeName, daysTo, status) {
        if (status === "Ended") {
            return qsTr("This show has ended")
        } else if (daysTo === "today") {
            return qsTr("%1 airs tonight").arg(episodeName)
        } else if (daysTo === "tomorrow") {
            return qsTr("%1 airs tomorrow").arg(episodeName)
        } else if (Boolean(episodeName.trim().length)) {
            return qsTr("%1 airs in %2 days").arg(episodeName).arg(daysTo)
        } else if (episodeName.trim().length === 0 && Boolean(daysTo.length)) {
            return qsTr("Next episode airs in %1 days").arg(daysTo)
        }

        return qsTr("No information about next episode")
    }

    SilicaFlickable {
        anchors.fill: parent

        PullDownMenu {
            busy: engine.Loading

            MenuItem {
                text: qsTr("Update All")
                onClicked: {
                    engine.SeriesListModel.updateAllSeries(settings.getUpdateEndedSeriesPreference())
                }
            }
        }

        SilicaListView {
            id: listView
            anchors.fill: parent
            model: engine.SeriesListModel.SeriesList

            header: PageHeader {
                id: header
                title: qsTr("My Series")
            }

            delegate: ListItem {
                id: listItem
                contentHeight: background.height + Theme.paddingMedium
                contentWidth: listView.width

                Rectangle {
                    id: container
                    anchors.fill: background
                    radius: 5
                    color: Theme.rgba(Theme.highlightBackgroundColor, 0.3)
                }

                BackgroundItem {
                    id: background
                    height: banner.height + seriesName.height + nextEpisode.height + Theme.paddingLarge
                    anchors.left: parent.left
                    anchors.leftMargin: Theme.paddingMedium
                    anchors.right: parent.right
                    anchors.rightMargin: Theme.paddingMedium
                    onClicked: {
                        engine.SeriesListModel.selectSeries(index)
                        pageStack.push(Qt.resolvedUrl("SeriesPage.qml"),
                                       {
                                           seriesId: ID,
                                           imdbId: IMDB_ID,
                                           seriesName: SeriesName,
                                           network: Network,
                                           banner: Banner,
                                           seriesStatus: Status,
                                           rating: Rating,
                                           genre: Genre,
                                           overview: Overview
                                       })
                    }
                }

                Column {
                    id: column
                    spacing: Theme.paddingSmall

                    SeriesBanner {
                        id: banner
                        bannerPath: Banner
                        sourceWidth: container.width
                    }

                    Label {
                        id: seriesName
                        text: SeriesName
                        anchors.left: parent.left
                        anchors.leftMargin: 2 * Theme.paddingMedium
                        font.pixelSize: Theme.fontSizeSmall
                    }

                    Label {
                        id: nextEpisode
                        text: nextEpisodeDetails(NextEpisodeName, DaysToNextEpisode, Status)
                        anchors.left: parent.left
                        anchors.leftMargin: 2 * Theme.paddingMedium
                        truncationMode: TruncationMode.Fade
                        width: column.width - Theme.paddingLarge
                        font.pixelSize: Theme.fontSizeTiny
                    }
                }
            }

            VerticalScrollDecorator {
                id: decorator
            }

            ViewPlaceholder {
                visible: listView.count === 0
                text: qsTr("Search for series first.")
                anchors.centerIn: listView
            }
        }
    }
}
