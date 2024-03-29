import QtQuick 2.0
import Sailfish.Silica 1.0

import '../components'

Page {

    id: statisticspage

    Component.onCompleted: {
        engine.Statistics.requestStatistics()
    }

    SilicaFlickable {

        anchors.fill: parent
        contentHeight: column.height

        Column {

            id: column
            width: parent.width

            PageHeader {
                title: qsTr("Statistics")
            }

            SectionHeader {
                text: qsTr("Numbers")
            }

            Row {
                width: parent.width

                Column {

                    width: parent.width / 2

                    TextField {
                        width: parent.width
                        readOnly: true
                        font.pixelSize: Theme.fontSizeSmall
                        text: engine.Statistics.WatchedEpisodesCount
                        label: qsTr("Watched episodes")
                    }

                    TextField {
                        width: parent.width
                        readOnly: true
                        font.pixelSize: Theme.fontSizeSmall
                        text: engine.Statistics.AllEpisodesCount
                        label: qsTr("All episodes")
                    }
                }

                Column {

                    width: parent.width / 2

                    ProgressCircle {

                        id: episodesProgress
                        value: isNaN(engine.Statistics.WatchedEpisodesCount / engine.Statistics.AllEpisodesCount) ? 0 : engine.Statistics.WatchedEpisodesCount / engine.Statistics.AllEpisodesCount;

                        anchors {
                            left: parent.left
                            leftMargin: (parent.width - episodesProgress.width) / 2
                        }

                    }
                }
            }

            Row {

                width: parent.width

                Column {

                    width: parent.width / 2

                    TextField {
                        width: parent.width
                        readOnly: true
                        font.pixelSize: Theme.fontSizeSmall
                        text: engine.Statistics.WatchedSeasonsCount
                        label: qsTr("Watched seasons")
                    }

                    TextField {
                        width: parent.width
                        readOnly: true
                        font.pixelSize: Theme.fontSizeSmall
                        text: engine.Statistics.AllSeasonsCount
                        label: qsTr("All seasons")
                    }
                }

                Column {

                    width: parent.width / 2

                    ProgressCircle {

                        id: seasonsProgress
                        value: isNaN(engine.Statistics.WatchedSeasonsCount / engine.Statistics.AllSeasonsCount) ? 0 : engine.Statistics.WatchedSeasonsCount / engine.Statistics.AllSeasonsCount;

                        anchors {
                            left: parent.left
                            leftMargin: (parent.width - seasonsProgress.width) / 2
                        }
                    }
                }
            }

            Row {

                width: parent.width

                Column {

                    width: parent.width / 2

                    TextField {
                        width: parent.width
                        readOnly: true
                        font.pixelSize: Theme.fontSizeSmall
                        text: engine.Statistics.WatchedSeriesCount
                        label: qsTr("Watched series")
                    }

                    TextField {
                        width: parent.width
                        readOnly: true
                        font.pixelSize: Theme.fontSizeSmall
                        text: engine.Statistics.AllSeriesCount
                        label: qsTr("All series")
                    }
                }

                Column {

                    width: parent.width / 2

                    ProgressCircle {

                        id: seriesProgress
                        value: isNaN(engine.Statistics.WatchedSeriesCount / engine.Statistics.AllSeriesCount) ? 0 : engine.Statistics.WatchedSeriesCount / engine.Statistics.AllSeriesCount;

                        anchors {
                            left: parent.left
                            leftMargin: (parent.width - seriesProgress.width) / 2
                        }
                    }
                }
            }

            SectionHeader {
                text: qsTr("Time")
            }

            TextField {
                width: statisticspage.width
                readOnly: true
                font.pixelSize: Theme.fontSizeSmall
                text: (engine.Statistics.WatchedEpisodesDuration / (24 * 60)).toFixed(2)
                label: qsTr("Days spent watching")
            }

            TextField {
                width: statisticspage.width
                readOnly: true
                font.pixelSize: Theme.fontSizeSmall
                text: (engine.Statistics.WatchedEpisodesDuration / 60).toFixed(2)
                label: qsTr("Hours spent watching")
            }

            TextField {
                width: statisticspage.width
                readOnly: true
                font.pixelSize: Theme.fontSizeSmall
                text: engine.Statistics.WatchedEpisodesDuration
                label: qsTr("Minutes spent watching")
            }

            TextField {
                width: statisticspage.width
                readOnly: true
                font.pixelSize: Theme.fontSizeSmall
                text: qsTr("%1 minutes").arg((engine.Statistics.AverageWatchedEpisodesDuration).toFixed(2))
                label: qsTr("Average episode duration")
            }
        }

        VerticalScrollDecorator { }
    }
}
