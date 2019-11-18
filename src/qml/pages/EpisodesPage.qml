import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: episodespage

    property int seriesId
    property int seasonNumber

    Component.onCompleted: initialize(seriesId, seasonNumber)

    function initialize(seriesId, seasonNumber) {
        engine.EpisodeListModel.getEpisodes(seriesId, seasonNumber)
    }

    SilicaListView {
        id: listView
        model: engine.EpisodeListModel.episodeList

        PullDownMenu {
            id: pulldownmenu
            enabled: listView.count > 0

            MenuItem {
                text: qsTr("I have seen these all")
                onClicked: {
                    engine.EpisodeListModel.markSeasonAsWatched(seriesId, seasonNumber)
                }
            }
        }

        header: Component {
            PageHeader {
                id: pageheader
                title: qsTr("Episodes")
            }
        }

        anchors.fill: parent

        delegate: ListItem {

            id: item

            contentHeight: background.height + Theme.paddingMedium
            contentWidth: listView.width

            Rectangle {
                id: container
                anchors.fill: background
                radius: 5
                color: Theme.rgba(Theme.highlightBackgroundColor, 0.3)
            }

            BackgroundItem {
                id: background
                height: episodeName.height + row.height + 10
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingMedium
                anchors.right: parent.right
                anchors.rightMargin: Theme.paddingMedium
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("EpisodeOverviewPage.qml"),
                                   { episodeBanner: Filename,
                                     episodeOverview: Overview,
                                     episodeName: EpisodeName,
                                     guestStars: GuestStars,
                                     writer: Writer,
                                     firstAired: FirstAired,
                                     watched: Watched,
                                     episodeId: ID,
                                     seriesId: seriesId,
                                     seasonNumber: SeasonNumber })

                }
            }

            Column {

                Row {
                    id: row
                    anchors.left: parent.left
                    anchors.leftMargin: 2 * Theme.paddingMedium

                    Label {
                        id: seasonNumber
                        text: SeasonNumber === 0
                              ? qsTr("Specials: Episode %1").arg(EpisodeNumber)
                              : qsTr("Season %1 Episode %2").arg(SeasonNumber).arg(EpisodeNumber)
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                    }
                }

                Label {
                    id: episodeName
                    text: EpisodeName
                    anchors.left: parent.left
                    anchors.leftMargin: 2 * Theme.paddingMedium
                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.primaryColor
                    truncationMode: TruncationMode.Fade
                    width: (episodespage.width - watched.width - 2 * Theme.paddingLarge)
                }
            }

            property string star: "image://theme/icon-m-favorite"
            property string favorite: "image://theme/icon-m-favorite-selected"

            Image {
                id: watched
                source: Watched === 0 ? star : favorite
                anchors.right: container.right
                anchors.rightMargin: Theme.paddingMedium
                anchors.top: container.top
                anchors.topMargin: Theme.paddingMedium
                cache: false

                MouseArea {
                    id: clickarea
                    onClicked: {
                        engine.EpisodeListModel.toggleWatched(ID, seriesId, SeasonNumber)
                    }
                    anchors.fill: parent
                }
            }
        }

        VerticalScrollDecorator {
            id: decorator
        }
    }

    ViewPlaceholder {
        enabled: listView.count === 0
        text: qsTr("No episodes.")
    }
}

