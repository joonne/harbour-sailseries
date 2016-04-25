import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    id: cover

    Label {
        id: placeholder
        text: qsTr("Nothing airs this week")
        font.pixelSize: Theme.fontSizeTiny
        color: Theme.secondaryColor
        anchors.centerIn: cover
        visible: listView.count === 0 && controller.SeriesListModel.Mode === "default"
    }

    //---------------------------------
    // This is the "MySeries" coverPage
    //---------------------------------

    function setPosterVisible() {
        if(controller.SeriesListModel.Mode === "mySeries" && controller.SeriesListModel.Poster !== null) {
            return "http://thetvdb.com/banners/" + controller.SeriesListModel.Poster
        }
    }

    Image {
        id: poster
        source: setPosterVisible()
        anchors.top: parent.top
        width: parent.width
        height: parent.height
        visible: controller.SeriesListModel.Mode === "mySeries"
        opacity: 1.0
    }

    //--------------------------------
    // This is the "default" coverPage
    //--------------------------------

    function defaultVisibility() {
        return controller.SeriesListModel.Mode === "default"
    }

    function getWeekday(weekday) {
        console.log(weekday)
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

    SilicaListView {
        visible: defaultVisibility()
        id: listView
        height: cover.height
        anchors.top: cover.top
        anchors.topMargin: Theme.paddingLarge
        model: controller.TodayModel.TodayModel

        delegate: ListItem {
            id: item
            contentHeight: column.height * 1.1
            contentWidth: listView.width

            Column {
                id: column
                x: Theme.paddingLarge

                Label {
                    id: seriesName
                    text: SeriesName
                    font.pixelSize: Theme.fontSizeMedium
                    color: Theme.primaryColor
                    truncationMode: TruncationMode.Fade
                    width: cover.width - Theme.paddingLarge
                }

                Label {
                    id: episodeNumber
                    text: qsTr("Season %1 Episode %2").arg(NextEpisodeSeasonNumber).arg(NextEpisodeNumber);
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.secondaryColor
                    truncationMode: TruncationMode.Fade
                    width: cover.width - Theme.paddingLarge
                    visible: listView.count < 3
                }

                Label {
                    id: episodeName
                    text: NextEpisodeName
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.secondaryColor
                    truncationMode: TruncationMode.Fade
                    width: cover.width - Theme.paddingLarge
                    visible: listView.count < 3
                }

                Label {
                    id: network
                    text: getWeekday(AirsDayOfWeek) + " " + AirsTime + " @ " + Network
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.secondaryColor
                    truncationMode: TruncationMode.Fade
                    width: cover.width - Theme.paddingLarge
                }
            }
        }
    }
}





// UPCOMING IDEAS

//            function search() {

//                appWindow.activate()

//                var page = pageStack.currentPage
//                if(page.contains("SearchPage")) {
//                    //appWindow.activate()
//                } else if(page.contains("MySeries") || page.contains("SeriesViewPage")) {
//                    pageStack.pop()
//                    pageStack.completeAnimation()
//                    pageStack.push(Qt.resolvedUrl("SearchPage.qml"))
//                } else if(page.contains("StartPage")) {
//                    pageStack.push(Qt.resolvedUrl("SearchPage.qml"))
//                }

//                console.log(page)
//            }
