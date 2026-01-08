import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: startpage

    function getWeekday(weekday) {
        switch(weekday) {
        case "Monday":
            return qsTr("Monday");
        case "Tuesday":
            return qsTr("Tuesday");
        case "Wednesday":
            return qsTr("Wednesday");
        case "Thursday":
            return qsTr("Thursday");
        case "Friday":
            return qsTr("Friday");
        case "Saturday":
            return qsTr("Saturday");
        case "Sunday":
            return qsTr("Sunday");
        default:
            return "-";
        }
    }

    SilicaFlickable {
        anchors.fill: parent

        PageHeader {
            id: header
            title: "SailSeries"
        }

        PullDownMenu {
            busy: engine.Loading

            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }

            MenuItem {
                text: qsTr("Statistics")
                onClicked: pageStack.push(Qt.resolvedUrl("StatisticsPage.qml"))
            }

            MenuItem {
                text: qsTr("Search for a Series")
                onClicked: pageStack.push(Qt.resolvedUrl("SearchPage.qml"))
            }

            MenuItem {
                text: qsTr("My Series")
                onClicked: pageStack.push(Qt.resolvedUrl("MySeriesPage.qml"))
            }
        }

        SectionHeader {
            id: sectionheader
            text: qsTr("This Week")
            visible: listView.count !== 0
            anchors {
                top: header.bottom
                topMargin: -(Theme.paddingLarge + Theme.paddingMedium)
            }
        }

        SilicaListView {
            id: listView
            height: startpage.height - header.height - sectionheader.height
            width: startpage.width
            clip: true
            anchors {
                top: sectionheader.bottom
            }
            model: engine.TodayModel.TodayModel

            section {
                property: "AirsDayOfWeek"
                criteria: ViewSection.FullString
                delegate: SectionHeader {
                    text: getWeekday(section)
                }
            }

            delegate: ListItem {
                id: item
                contentHeight: column.height * 1.1
                contentWidth: listView.width
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("EpisodePage.qml"),
                                   { episodeOverview: NextEpisodeOverview,
                                       episodeName: NextEpisodeName,
                                       guestStars: NextEpisodeGuestStars,
                                       writers: NextEpisodeWriter,
                                       firstAired: NextEpisodeFirstAired,
                                       watched: NextEpisodeWatched,
                                       episodeId: NextEpisodeId,
                                       seriesId: ID,
                                       seasonNumber: NextEpisodeSeasonNumber,
                                       filename: NextEpisodeFilename });
                }

                Column {
                    id: column
                    x: Theme.paddingLarge

                    Label {
                        id: seriesName
                        text: SeriesName
                        color: Theme.primaryColor
                    }

                    Label {
                        id: episodeNumber
                        text: NextEpisodeSeasonNumber == 0
                              ? qsTr("Special Episode %1").arg(NextEpisodeNumber)
                              : qsTr("Season %1 Episode %2").arg(NextEpisodeSeasonNumber).arg(NextEpisodeNumber);
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                    }

                    Label {
                        id: episodeName
                        text: NextEpisodeName
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                    }


                    Label {
                        id: network
                        text: AirsTime + (Network ? " @ " + Network : "")
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                    }
                }
            }

            VerticalScrollDecorator {
                id: decorator
            }

            ViewPlaceholder {
                enabled: listView.count === 0
                text: qsTr("Nothing airs this week")
                anchors.centerIn: listView
            }
        }
    }
}

