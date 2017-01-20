import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: startpage

    function checkNotification(weekday, seriesName, episodeName, airsTime) {
        if (weekday === new Date().toLocaleString(Qt.locale("en_EN"), "dddd")) {
            var summary = seriesName;
            var body = episodeName ? episodeName + '\n' + airsTime : airsTime;
            var previewSummary = seriesName;
            var previewBody = episodeName;
            notificationhandler.publish(summary, body, previewSummary, previewBody);
        }
    }

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

            MenuItem {
                text: qsTr("About")
                onClicked: pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
            }

            MenuItem {
                text: qsTr("Statistics")
                onClicked: pageStack.push(Qt.resolvedUrl("StatisticsPage.qml"))
            }

            MenuItem {
                text: qsTr("Settings")
                onClicked: pageStack.push(Qt.resolvedUrl("SettingsPage.qml"))
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
                    pageStack.push(Qt.resolvedUrl("EpisodeOverviewPage.qml"),
                                   { episodeBanner: NextEpisodeBanner,
                                       episodeOverview: NextEpisodeOverview,
                                       episodeName: NextEpisodeName,
                                       guestStars: NextEpisodeGuestStars,
                                       writer: NextEpisodeWriter,
                                       firstAired: NextEpisodeFirstAired,
                                       watched: NextEpisodeWatched,
                                       episodeId: NextEpisodeId,
                                       seriesId: ID });
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
                        text: qsTr("Season %1 Episode %2").arg(NextEpisodeSeasonNumber).arg(NextEpisodeNumber);
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
                        text: AirsTime + " @ " + Network
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                    }
                }

                Component.onCompleted: {
                    if (settings.getNotificationPreference())
                        checkNotification(AirsDayOfWeek, SeriesName, NextEpisodeName, AirsTime)
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

            BusyIndicator {
                size: BusyIndicatorSize.Large
                anchors.centerIn: parent
                running: engine.Loading
            }
        }
    }
}

