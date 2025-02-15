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
    property string filename
    property string writers
    property string guestStars

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
                source: filename
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
                            engine.EpisodeListModel.setWatched(episodeId, seriesId, watched)
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
        }
    }
}

