import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    id: cover

    Label {
        id: placeholder
        text: "Nothing airs today"
        font.pixelSize: Theme.fontSizeTiny
        anchors.centerIn: cover
        visible: listView.count === 0
    }

    //    function nextEpisodeDetails(episodeName,daysTo,status) {

    //        if(status === "Ended") {
    //            return "This show has ended";
    //        } else if(daysTo === "today") {
    //            return episodeName + " airs today";
    //        } else if(daysTo === "tommorrow") {
    //            return episodeName + " airs tomorrow";
    //        } else if(episodeName.length !== 0) {
    //            return episodeName + " airs in " + daysTo + " days";
    //        } else if(episodeName.length === 0 && daysTo !== "unknown") {
    //            return "Next episode airs in " + daysTo + " days";
    //        } else {
    //            return "No information about next episode";
    //        }
    //    }

    //            function noPosterVisible() {
    //                var visible
    //                if(controller.SeriesListModel.Poster.length !== 0 && controller.SeriesListModel.Mode === "mySeries") {
    //                    visible = true
    //                } else {
    //                    visible = false
    //                }
    //                return visible
    //            }

    //            function search(page) {
    //                if(page.contains("SearchPage")) {
    //                    return true;
    //                }
    //            }


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

    //            function setPosterVisible() {
    //                if(appWindow.applicationActive === "false" && controller.SeriesListModel.Mode === "mySeries") {
    //                    poster.source = "http://thetvdb.com/banners/" + controller.SeriesListModel.Poster
    //                }
    //            }

    //    Image {
    //        id: poster
    //        source: controller.SeriesListModel.Poster.length === 0 ? source = null : source = "http://thetvdb.com/banners/" + controller.SeriesListModel.Poster
    //        anchors.top: parent.top
    //        width: parent.width
    //        height: parent.height
    //        visible: controller.SeriesListModel.Mode === "mySeries"
    //        opacity: 0.3
    //    }

    //    Label {
    //        id: nextEpisode
    //        text: "episodeName";
    //            /*controller.SeriesListModel.Mode === "mySeries" ? text = nextEpisodeDetails(controller.SeriesListModel.NextEpisodeName,
    //                                                                                         controller.SeriesListModel.DaysToNextEpisode,
    //                                                                                         controller.SeriesListModel.Status) :
    //                                                               text = "";*/
    //        anchors.left: parent.left
    //        anchors.leftMargin: 2 * Theme.paddingMedium
    //        anchors.bottom: parent.bottom
    //        anchors.bottomMargin: 2 * Theme.paddingMedium
    //        font.pixelSize: Theme.fontSizeExtraSmall
    //        color: Theme.primaryColor
    //    }

    //            Label {
    //                id: noPoster
    //                text: "No poster."
    //                anchors.centerIn: parent
    //                visible: noPosterVisible()
    //            }

    //    Label {
    //        id: placeholder
    //        text: "SailSeries"
    //        anchors.centerIn: cover
    //        visible: controller.SeriesListModel.Mode === "default" ? true : false
    //    }

    //            CoverActionList {
    //                id: mySeriesCoverActionList
    //                enabled: controller.SeriesListModel.Mode === "mySeries"

    //                CoverAction {
    //                    iconSource: "image://theme/icon-cover-next"
    //                    onTriggered: {
    //                        console.log("seuraava")
    //                        controller.SeriesListModel.nextPoster();
    //                        poster.source = "http://thetvdb.com/banners/" + controller.SeriesListModel.Poster
    //                        console.log(controller.SeriesListModel.Poster)
    //                    }
    //                }

    //                CoverAction {
    //                    iconSource: "image://theme/icon-cover-search"
    //                    onTriggered: {
    //                        search()
    //                    }
    //                }
    //       }

    //        CoverActionList {
    //            id: defaultList
    //            enabled: controller.SeriesListModel.Mode === "default"

    //                CoverAction {
    //                    iconSource: "image://theme/icon-cover-search"
    //                    onTriggered: {
    //                        console.log("etsi")
    //                        // TODO stack reformation
    //                        search()
    //                    }
    //                }

    //        }

    //--------------------------------
    // This is the "default" coverPage
    //--------------------------------

    Label {
        id: defaultheader
        text: "Today"
        font.pixelSize: Theme.fontSizeSmall
        anchors.top: cover.top
        anchors.topMargin: Theme.paddingLarge
        anchors.left: cover.left
        anchors.leftMargin: Theme.paddingMedium
        visible: listView.count !== 0
    }

    SilicaListView {
        visible: controller.SeriesListModel.Mode === "default" ? visible = true : visible = false
        id: listView
        //anchors.fill: parent
        anchors.top: defaultheader.bottom
        anchors.topMargin: Theme.paddingLarge
        model: controller.TodayModel.TodayModel

        //        header: Label {
        //            text: "TONIGHT"
        //            width: listView.width
        //            color: Theme.primaryColor
        //            font.pixelSize: Theme.fontSizeSmall
        //        }

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

                //                Label {
                //                    id: episodeNumber
                //                    text: "Season " + NextEpisodeSeasonNumber + " Episode " + NextEpisodeNumber
                //                    font.pixelSize: Theme.fontSizeTiny
                //                    color: Theme.secondaryColor
                //                }

                Label {
                    id: episodeName
                    text: NextEpisodeName
                    font.pixelSize: Theme.fontSizeTiny
                    color: Theme.secondaryColor
                }

                //                Label {
                //                    id: network
                //                    text: AirsTime + " @ " + Network
                //                    font.pixelSize: Theme.fontSizeTiny
                //                    color: Theme.secondaryColor
                //                }

            }
        }
    }
}
