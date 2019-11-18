import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components"

Page {

    id: episodeoverviewpage

    // these are pushed from previous page with the pageStack
    property string episodeOverview
    property string episodeName
    property string firstAired
    property bool watched
    property int episodeId
    property int seriesId
    property int seasonNumber

    Connections {
        target: engine
        onUpdateEpisodeDetails: {
            banner.source = "http://thetvdb.com/banners/" + episode.filename
            writerField.text = episode.writers.join(", ")
            guestStarsField.text = episode.guestStars.join(", ")
        }
    }

    Component.onCompleted: {
        engine.requestEpisodeDetails(episodeId)
    }

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
                anchors.left: parent.left
                anchors.leftMargin: (episodeoverviewpage.width - banner.width) / 2

                onStatusChanged: {
                    var fallback = "qrc:///images/episode-image-fallback.jpg";
                    if (status === Image.Error || status === Image.Null) {
                        source = fallback
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
                    label: qsTr("Original air date")
                    width: parent.width - watched_image.width - Theme.paddingLarge

                }

                Image {
                    id: watched_image
                    source: watched ? "image://theme/icon-m-favorite-selected" : "image://theme/icon-m-favorite"

                    MouseArea {
                        id: clickarea
                        anchors.fill: parent
                        onClicked: {
                            watched = !watched
                            engine.EpisodeListModel.toggleWatched(episodeId, seriesId, seasonNumber)
                        }
                    }
                }
            }

            TextArea {
                id: overviewField
                width: episodeoverviewpage.width
                label: qsTr("Overview")
                text:  episodeOverview
                readOnly: true
            }

            TextArea {
                id: writerField
                width: episodeoverviewpage.width
                label: qsTr("Writer")
                readOnly: true
            }

            TextArea {
                id: guestStarsField
                width: episodeoverviewpage.width
                label: qsTr("Guest Stars")
                readOnly: true
            }
        }
    }
}

