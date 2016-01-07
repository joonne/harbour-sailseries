import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: episodespage

    property string seriesID
    property int seasonNumber

    Component.onCompleted: {
        initialize(seriesID, seasonNumber)
    }

    Component.onDestruction: {
        pageStack.find(function(page) {
            return page.pageName === "SeriesViewPage"
        }).updateModel();
    }


    function initialize(seriesID, seasonNumber) {
        controller.EpisodeListModel.populateEpisodeList(seriesID, seasonNumber)
        listView.model = controller.EpisodeListModel.episodeList
    }

    SilicaListView {
        id: listView
        model: controller.EpisodeListModel.episodeList

        PullDownMenu {
            id: pulldownmenu

            MenuItem {
                text: qsTr("I have seen these all")
                onClicked: {
                    controller.EpisodeListModel.markSeasonWatched(seriesID, seasonNumber)
                    controller.SeasonListModel.populateSeasonList(seriesID)
                    pageStack.find(function(page) {
                        return page.pageName === "SeriesViewPage"
                    }).updateModel();
                    pageStack.pop()
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
                                     firstAired: FirstAired })

                }
            }

            Column {

                Row {
                    id: row
                    anchors.left: parent.left
                    anchors.leftMargin: 2 * Theme.paddingMedium

                    Label {
                        id: seasonNumber
                        text: qsTr("Season") + " " + SeasonNumber
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                    }

                    Label {
                        id: episodeNumber
                        text: " " + qsTr("Episode") + " " + EpisodeNumber
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
                }
            }

            property string star: "image://theme/icon-l-star"
            property string favorite: "image://theme/icon-l-favorite"

            function setSource() {

                if(Watched === 0) {
                    return star;
                } else {
                    return favorite;
                }
            }

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
                        Watched === 0 ? Watched = 1 : Watched = 0
                        controller.EpisodeListModel.toggleWatched(ID)
                        controller.SeasonListModel.populateSeasonList(seriesID)
                        pageStack.find(function(page) {
                            return page.pageName === "SeriesViewPage"
                        }).updateModel();
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

