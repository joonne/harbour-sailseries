import QtQuick 2.0
import Sailfish.Silica 1.0

import '../components'

Page {

    id: statisticspage

    Component.onCompleted: engine.Statistics.updateStatistics()

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
                text: qsTr("%1 days").arg((engine.Statistics.WatchedEpisodesDuration / (24 * 60)).toFixed(2))
                label: qsTr("Time spent watching")
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
                text: engine.Statistics.AllSeasonsCount
                label: qsTr("All seasons")
            }

            ProgressCircleBase {
                id: progress
                anchors {
                    left: parent.left
                    leftMargin: (parent.width - progress.width) / 2
                }
                height: 150
                width: 150
                implicitWidth: Theme.itemSizeHuge
                borderWidth: Math.round(Theme.paddingLarge/3)
                backgroundColor: Theme.rgba(Theme.highlightColor, 0.2)
                progressColor: Theme.rgba(Theme.highlightColor, 0.4)
                progressValue: engine.Statistics.WatchedEpisodesCount / engine.Statistics.AllEpisodesCount

                Text {
                    anchors.centerIn: progress
                    text: qsTr("%1 %").arg(Math.round((engine.Statistics.WatchedEpisodesCount / engine.Statistics.AllEpisodesCount) * 100))
                    font.pixelSize: Theme.fontSizeTiny
                    color: Theme.secondaryColor
                }
            }
        }

        VerticalScrollDecorator { }
    }
}
