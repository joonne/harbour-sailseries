import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components"

Page {

    id: episodepage

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
    property int runtime

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
                width: parent.width - (Theme.paddingLarge * 2)
                height: (9/16) * banner.width
                anchors.left: parent.left
                anchors.leftMargin: (episodepage.width - banner.width) / 2
            }

            Item {
                id: padding
                height: Theme.paddingLarge
                width: episodepage.width
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

            TextField {
                id: runtimeField
                text: qsTr("%1 minutes").arg(runtime)
                readOnly: true
                label: qsTr("Runtime")
                width: parent.width - Theme.paddingLarge
                visible: runtime && runtime !== 0
            }

            TextArea {
                id: overviewField
                width: episodepage.width
                label: qsTr("Overview")
                text:  episodeOverview
                readOnly: true
            }
        }
    }
}

