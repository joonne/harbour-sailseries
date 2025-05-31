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

    Image {
        id: coverBackgroundImage
        asynchronous: true
        fillMode: Image.PreserveAspectFit
        width: parent.height - Theme.paddingLarge
        height: width
        opacity: 0.15
        source: "qrc:///harbour-sailseries.svg"
        property int sourceDimension: Math.min(width, height) - Theme.paddingMedium
        anchors {
            centerIn: undefined
            verticalCenter: parent.verticalCenter
            bottom: parent.bottom
            bottomMargin: Theme.paddingMedium
            right: parent.right
            rightMargin: Theme.paddingMedium
        }
        sourceSize {
            width: sourceDimension
            height: sourceDimension
        }
        visible: isDefaultMode()
    }

    Label {
        id: placeholder
        text: qsTr("Nothing airs this week")
        wrapMode: "WordWrap"
        font.pixelSize: Theme.fontSizeSmall
        color: Theme.primaryColor
        anchors {
            centerIn: cover
        }
        visible: listView.count === 0 && isDefaultMode()
        width: cover.width - (2 * Theme.paddingLarge)
    }

    //---------------------------------
    // This is the "my_series" coverPage
    //---------------------------------

    function setPosterVisible(aPoster) {
        if (isMySeriesMode() && aPoster) {
            return aPoster;
        }
    }

    Image {
        id: poster
        anchors.top: parent.top
        width: parent.width
        height: parent.height
        visible: isMySeriesMode()
        opacity: 1.0
    }

    //--------------------------------
    // This is the "default" coverPage
    //--------------------------------

    function isDefaultMode() {
        return engine.Mode === "default"
    }

    function isMySeriesMode() {
        return engine.Mode === "my_series"
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
        visible: isDefaultMode()
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
                    text: NextEpisodeSeasonNumber == 0
                          ? qsTr("Special Episode %1").arg(NextEpisodeNumber)
                          : qsTr("Season %1 Episode %2").arg(NextEpisodeSeasonNumber).arg(NextEpisodeNumber);
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
                    text: getWeekday(AirsDayOfWeek) + " " + AirsTime + (Network ? " @ " + Network : "")
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.secondaryColor
                    truncationMode: TruncationMode.Fade
                    width: cover.width - Theme.paddingLarge
                }
            }
        }
    }
}
