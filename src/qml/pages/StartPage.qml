import QtQuick 2.0
import Sailfish.Silica 1.0
import org.nemomobile.notifications 1.0

Page {
    id: startpage

//    Notification {
//        id: notification
//        category: "x-nemo.example"
//        summary: "Game Of Thrones"
//        body: "Book of the Stranger\n21:00"
//        previewSummary: "Game Of Thrones"
//        previewBody: "21:00"
//        timestamp: "2016-05-13 21:20:00"
//        onClicked: activate()
//        remoteActions: [ {
//                "name": "default",
//                "displayName": "Do something",
//                "icon": "icon-s-do-it",
//                "service": "org.nemomobile.example",
//                "path": "/example",
//                "iface": "org.nemomobile.example",
//                "method": "doSomething",
//                "arguments": [ "argument", 1 ]
//            } ]
//    }

//    function activate() {
//        appWindow.activate()
//    }

//    Component.onCompleted: timer.start()

//    Timer {
//        id: timer

//        interval: 10000
//        repeat: true
//        onTriggered: notification.publish()
//    }

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
                font.pixelSize: Theme.fontSizeSmall
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("AboutPage.qml"))
                }
            }

            MenuItem {
                text: qsTr("Search for a Series")
                font.pixelSize: Theme.fontSizeSmall
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("SearchPage.qml"))
                }
            }

            MenuItem {
                text: qsTr("My Series")
                font.pixelSize: Theme.fontSizeSmall
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("MySeriesPage.qml"))
                }
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
            model: controller.TodayModel.TodayModel

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
                                       firstAired: NextEpisodeFirstAired })
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

