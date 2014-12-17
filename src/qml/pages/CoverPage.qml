import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
        id: cover

        function noPosterVisible() {
            var visible
            if(LISTMODEL.Poster.length === 0 && LISTMODEL.Mode === "mySeries") {
                visible = true
            } else {
                visible = false
            }
            return visible
        }

        Image {
            id: poster
            source: LISTMODEL.Poster.length === 0 ? source = null : source = "http://thetvdb.com/banners/" + LISTMODEL.Poster
            anchors.top: parent.top
            width: parent.width
            height: parent.height
            visible: LISTMODEL.Mode === "mySeries"
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
            visible: LISTMODEL.Mode === "default" ? true : false
        }

        CoverActionList {
            id: mySeriesCoverActionList
            enabled: LISTMODEL.Mode === "mySeries"

            CoverAction {
                iconSource: "image://theme/icon-cover-next"
                onTriggered: {
                    console.log("seuraava")
                    LISTMODEL.nextPoster();
                    poster.source = "http://thetvdb.com/banners/" + LISTMODEL.Poster
                    console.log(LISTMODEL.Poster)
                }
            }

            CoverAction {
                iconSource: "image://theme/icon-cover-search"
                onTriggered: {
                    console.log("etsi")
                    // TODO stack reformation
                    //pageStack.push(Qt.resolvedUrl("pages/SearchPage.qml"))
                    appWindow.activate()
                }
            }
        }

        CoverActionList {
            id: defaultList
            enabled: LISTMODEL.Mode === "default"

            CoverAction {
                iconSource: "image://theme/icon-cover-search"
                onTriggered: {
                    console.log("etsi")
                    // TODO stack reformation
                    //pageStack.push(Qt.resolvedUrl("pages/SearchPage.qml"))
                    appWindow.activate()
                }
            }

        }

    }
