import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    id: cover

    Connections {
        target: engine.SeriesListModel
        onPosterChanged: {
            poster.source = setPosterVisible(aPoster)
        }
    }

    Label {
        id: placeholder
        text: qsTr("Nothing airs this week")
        font.pixelSize: Theme.fontSizeTiny
        color: Theme.secondaryColor
        anchors.centerIn: cover
        visible: listView.count === 0 && engine.Mode === "default"
    }

    //---------------------------------
    // This is the "m_series" coverPage
    //---------------------------------

    function setPosterVisible(aPoster) {
        if (engine.Mode === "m_series" && aPoster) {
            return "http://thetvdb.com/banners/" + aPoster;
        }
    }

    Image {
        id: poster
        anchors.top: parent.top
        width: parent.width
        height: parent.height
        visible: engine.Mode === "m_series"
        opacity: 1.0
    }

    //--------------------------------
    // This is the "default" coverPage
    //--------------------------------

    function defaultVisibility() {
        return engine.Mode === "default"
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

    SilicaListView {
        id: listView
        visible: defaultVisibility()
        height: cover.height
        anchors {
            top: cover.top
            topMargin: Theme.paddingLarge
        }
        spacing: Theme.paddingMedium
        model: engine.TodayModel.TodayModel

        delegate: ListItem {
            id: item
            contentHeight: listView.count > 4 ? (Theme.coverSizeLarge.height / 5) : column.height
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
                    visible: listView.count < 3 && text.length > 0
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
