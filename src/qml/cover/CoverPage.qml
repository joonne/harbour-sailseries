import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
        id: cover

        function noPosterVisible() {
            var visible
            if(DATAMODEL.SeriesListModel.Poster.length === 0 && DATAMODEL.SeriesListModel.Mode === "mySeries") {
                visible = true
            } else {
                visible = false
            }
            return visible
        }

        function search() {

            appWindow.activate()

            var page = pageStack.currentPage
            if(page.contains("SearchPage")) {
                //appWindow.activate()
            } else if(page.contains("MySeries") || page.contains("SeriesViewPage")) {
                pageStack.pop()
                pageStack.completeAnimation()
                pageStack.push(Qt.resolvedUrl("SearchPage.qml"))
            } else if(page.contains("StartPage")) {
                pageStack.push(Qt.resolvedUrl("SearchPage.qml"))
            }

            console.log(page)
        }

        function setPosterVisible() {
            if(appWindow.applicationActive === "false" && DATAMODEL.SeriesListModel.Mode === "mySeries") {
                poster.source = "http://thetvdb.com/banners/" + DATAMODEL.SeriesListModel.Poster
            }
        }

        Image {
            id: poster
            source: DATAMODEL.SeriesListModel.Poster.length === 0 ? source = null : source = "http://thetvdb.com/banners/" + DATAMODEL.SeriesListModel.Poster
            anchors.top: parent.top
            width: parent.width
            height: parent.height
            visible: DATAMODEL.SeriesListModel.Mode === "mySeries"
            opacity: 0.6
        }

        Label {
            id: noPoster
            text: "No poster."
            anchors.centerIn: parent
            visible: noPosterVisible()
        }

        Label {
            id: placeholder
            text: "SailSeries"
            anchors.centerIn: cover
            visible: DATAMODEL.SeriesListModel.Mode === "default" ? true : false
        }

        CoverActionList {
            id: mySeriesCoverActionList
            enabled: DATAMODEL.SeriesListModel.Mode === "mySeries"

            CoverAction {
                iconSource: "image://theme/icon-cover-next"
                onTriggered: {
                    console.log("seuraava")
                    DATAMODEL.SeriesListModel.nextPoster();
                    poster.source = "http://thetvdb.com/banners/" + DATAMODEL.SeriesListModel.Poster
                    console.log(DATAMODEL.SeriesListModel.Poster)
                }
            }

            CoverAction {
                iconSource: "image://theme/icon-cover-search"
                onTriggered: {
                    search()
                }
            }
        }

        CoverActionList {
            id: defaultList
            enabled: DATAMODEL.SeriesListModel.Mode === "default"

            CoverAction {
                iconSource: "image://theme/icon-cover-search"
                onTriggered: {
                    console.log("etsi")
                    // TODO stack reformation
                    search()
                }
            }

        }

    }
