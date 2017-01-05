import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components"

Page {
    id: episodeoverviewpage

    // processes tubes "|" out of the given string
    function process(string) {
        if (string.charAt(0) === "|" && string.charAt(string.length - 1) === "|") {
            var newstring = string.split("|").join(", ")
            return newstring.substr(2,(newstring.length - 4))
        } else if (string.charAt(0) === "|") {
            return string.split("|").join(", ").substr(2)
        } else {
            return string.split("|").join(", ")
        }
    }

    // these are pushed from previous page with the pageStack
    property string episodeBanner
    property string episodeOverview
    property string episodeName
    property string guestStars
    property string writer
    property string firstAired
    property int watched
    property string episodeId
    property string seriesId

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            anchors.top: parent.top
            anchors.topMargin: Theme.paddingMedium

            PageHeader {
                id: header
                title: episodeName
            }

            Image {
                id: banner
                source: "http://thetvdb.com/banners/" + episodeBanner
                anchors.left: parent.left
                anchors.leftMargin: (episodeoverviewpage.width - banner.width) / 2

                onStatusChanged: {
                    var fallback = "qrc:///images/episode-image-fallback.jpg";
                    if (status === Image.Error || status === Image.Null) {
                        source = fallback;
                    }
                }
            }

            Item {
                id: padding
                height: Theme.paddingLarge
                width: episodeoverviewpage.width
            }

            Row {
                width: parent.width

                TextField {
                    id: firstAiredField
                    text: firstAired
                    readOnly: true
                    color: Theme.secondaryColor
                    label: qsTr("Original air date")
                    font.pixelSize: Theme.fontSizeSmall
                    width: parent.width - watched_image.width - Theme.paddingLarge

                }

                Image {
                    id: watched_image
                    source: watched === 0 ? "image://theme/icon-m-favorite" : "image://theme/icon-m-favorite-selected"

                    MouseArea {
                        id: clickarea
                        anchors.fill: parent
                        onClicked: {
                            watched === 1 ? watched = 0 : watched = 1;
                            console.log("episodeId", episodeId);
                            console.log("seriesId", seriesId);
                            console.log("watched", watched);
                            engine.EpisodeListModel.toggleWatched(episodeId);
                            engine.SeasonListModel.populateSeasonList(seriesId);
                            engine.TodayModel.populateTodayModel();
                        }
                    }
                }
            }

            TextExpander {
                id: expander
                width: episodeoverviewpage.width
                textContent: episodeOverview
            }

            TextArea {
                id: writerField
                width: episodeoverviewpage.width
                text: process(writer)
                label: qsTr("Writer")
                color: Theme.secondaryColor
                readOnly: true
            }

            TextArea {
                id: guestStarsField
                width: episodeoverviewpage.width
                text: process(guestStars)
                label: qsTr("Guest Stars")
                color: Theme.secondaryColor
                readOnly: true
            }
        }
    }
}

