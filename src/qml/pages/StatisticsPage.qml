import QtQuick 2.0
import Sailfish.Silica 1.0

import '../components'

Page {

    id: statisticspage

    Component.onCompleted: engine.Statistics.updateStatistics()

    function formatMinutes(duration) {
        return duration === 1 ? qsTr("%1 minute").arg(duration) : qsTr("%1 minutes").arg(duration)
    }

    function formatDays(duration) {
        return duration === 1 ? qsTr("%1 day").arg(duration) : qsTr("%1 days").arg(duration)
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
                text: qsTr("Time")
            }

            TextField {
                width: statisticspage.width
                readOnly: true
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: (engine.Statistics.WatchedEpisodesDuration / (24 * 60)).toFixed(2)
                label: qsTr("Days spent watching")
            }

            TextField {
                width: statisticspage.width
                readOnly: true
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: (engine.Statistics.WatchedEpisodesDuration / 60).toFixed(2)
                label: qsTr("Hours spent watching")
            }

            TextField {
                width: statisticspage.width
                readOnly: true
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: engine.Statistics.WatchedEpisodesDuration
                label: qsTr("Minutes spent watching")
            }

            TextField {
                width: statisticspage.width
                readOnly: true
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: formatMinutes((engine.Statistics.AverageWatchedEpisodesDuration).toFixed(2))
                label: qsTr("Average episode duration")
            }

            SectionHeader {
                text: qsTr("Numbers")
            }

            TextField {
                width: statisticspage.width
                readOnly: true
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: engine.Statistics.WatchedEpisodesCount
                label: qsTr("Watched episodes")
            }

            TextField {
                width: statisticspage.width
                readOnly: true
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: engine.Statistics.AllEpisodesCount
                label: qsTr("All episodes")
            }

            TextField {
                width: statisticspage.width
                readOnly: true
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: engine.Statistics.AllSeriesCount
                label: qsTr("All series")
            }

            TextField {
                width: statisticspage.width
                readOnly: true
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: engine.Statistics.WatchedSeasonsCount
                label: qsTr("Watched seasons")
            }

            TextField {
                width: statisticspage.width
                readOnly: true
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: engine.Statistics.AllSeasonsCount
                label: qsTr("All seasons")
            }

            ProgressCircleBase {
                id: episodesProgress
                anchors {
                    left: parent.left
                    leftMargin: (parent.width - episodesProgress.width) / 2
                }
                height: 150
                width: 150
                implicitWidth: Theme.itemSizeHuge
                borderWidth: Math.round(Theme.paddingLarge/3)
                backgroundColor: Theme.rgba(Theme.highlightColor, 0.2)
                progressColor: Theme.rgba(Theme.highlightColor, 0.4)
                progressValue: engine.Statistics.WatchedEpisodesCount / engine.Statistics.AllEpisodesCount

                Text {
                    anchors.centerIn: episodesProgress
                    text: qsTr("%1 %").arg(Math.round((engine.Statistics.WatchedEpisodesCount / engine.Statistics.AllEpisodesCount) * 100))
                    font.pixelSize: Theme.fontSizeTiny
                    color: Theme.secondaryColor
                }
            }

            ProgressCircleBase {
                id: seasonsProgress
                anchors {
                    left: parent.left
                    leftMargin: (parent.width - seasonsProgress.width) / 2
                }
                height: 150
                width: 150
                implicitWidth: Theme.itemSizeHuge
                borderWidth: Math.round(Theme.paddingLarge/3)
                backgroundColor: Theme.rgba(Theme.highlightColor, 0.2)
                progressColor: Theme.rgba(Theme.highlightColor, 0.4)
                progressValue: engine.Statistics.WatchedSeasonsCount / engine.Statistics.AllSeasonsCount

                Text {
                    anchors.centerIn: seasonsProgress
                    text: qsTr("%1 %").arg(Math.round((engine.Statistics.WatchedSeasonsCount / engine.Statistics.AllSeasonsCount) * 100))
                    font.pixelSize: Theme.fontSizeTiny
                    color: Theme.secondaryColor
                }
            }
        }

        VerticalScrollDecorator { }
    }
}
