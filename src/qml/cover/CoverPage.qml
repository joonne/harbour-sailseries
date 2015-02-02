import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    id: cover

    Label {
        id: placeholder
        text: "Nothing airs today"
        font.pixelSize: Theme.fontSizeTiny
        anchors.centerIn: cover
        visible: listView.count === 0 && controller.SeriesListModel.Mode === "default"
    }

    //---------------------------------
    // This is the "MySeries" coverPage
    //---------------------------------

    function nextEpisodeDetails(episodeName,daysTo,status) {

        if(status === "Ended") {
            return "This show has ended";
        } else if(daysTo === "today") {
            return episodeName + " airs today";
        } else if(daysTo === "tommorrow") {
            return episodeName + " airs tomorrow";
        } else if(episodeName.length !== 0) {
            return episodeName + " airs in " + daysTo + " days";
        } else if(episodeName.length === 0 && daysTo !== "unknown") {
            return "Next episode airs in " + daysTo + " days";
        } else {
            return "No information about next episode";
        }
    }

    function setDetails() {

        if(controller.SeriesListModel.Mode === "mySeries" &&
                controller.SeriesListModel.NextEpisodeName !== null &&
                controller.SeriesListModel.DaysToNextEpisode !== null &&
                controller.SeriesListModel.Status !== null) {
            console.log("pitäis toimia.")
            return nextEpisodeDetails(controller.SeriesListModel.NextEpisodeName,controller.SeriesListModel.DaysToNextEpisode,controller.SeriesListModel.Status)
        }
    }

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
        opacity: 0.6
    }

    Label {
        id: nextEpisode
        text: "experimental"
        width: cover.width
        anchors.left: parent.left
        anchors.leftMargin: 2 * Theme.paddingMedium
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 2 * Theme.paddingMedium
        font.pixelSize: Theme.fontSizeTiny
        color: Theme.primaryColor
        visible: false
    }

    //--------------------------------
    // This is the "default" coverPage
    //--------------------------------

    function defaultVisibility() {
        var ret = false;
        if(controller.SeriesListModel.Mode === "default") {
            ret = true;
        }
        return ret;
    }

    Label {
        id: defaultheader
        text: "Today"
        color: Theme.primaryColor
        font.pixelSize: Theme.fontSizeMedium
        anchors.top: cover.top
        anchors.topMargin: Theme.paddingLarge
        anchors.left: cover.left
        anchors.leftMargin: Theme.paddingMedium
        visible: listView.count !== 0 && defaultVisibility()
    }

    SilicaListView {
        visible: defaultVisibility()
        id: listView
        height: 500
        anchors.top: defaultheader.bottom
        anchors.topMargin: Theme.paddingLarge
        model: controller.TodayModel.TodayModel

        delegate: ListItem {
            id: item
            contentHeight: column.height * 1.1
            contentWidth: listView.width

            Column {
                id: column
                x: Theme.paddingMedium

                Label {
                    id: seriesName
                    text: SeriesName.length === 0 ? text = "SeriesName" : text = SeriesName
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.primaryColor
                }

                Label {
                    id: episodeName
                    text: NextEpisodeName
                    font.pixelSize: Theme.fontSizeTiny
                    color: Theme.secondaryColor
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
