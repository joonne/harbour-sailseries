import QtQuick 2.0
import Sailfish.Silica 1.0

Item {

    height: seriesView.height;
    width: seriesView.width;

    property string seriesID

    // initialization is too fast without this
    Timer {
        id: timer
        interval: 600
        onTriggered: initialize(seriesID)
    }

    Component.onCompleted: timer.start()

    function initialize(seriesID) {
        controller.EpisodeListModel.populateEpisodeList(seriesID)
        listView.model = controller.EpisodeListModel.episodeList
        updateSource()
    }

    SilicaListView {
        id: listView
        model: controller.EpisodeListModel.episodeList

        header: Component {
            PageHeader {
                id: pageheader
                title: "Episodes"
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
                    episodeOverview.episodeBanner = Filename
                    episodeOverview.episodeOverview = Overview
                    episodeOverview.episodeName = EpisodeName
                    episodeOverview.visible = true
                    console.log(Filename)

                }
            }

            Column {

                Row {
                    id: row
                    anchors.left: parent.left
                    anchors.leftMargin: 2 * Theme.paddingMedium

                    Label {
                        id: seasonNumber
                        text: qsTr("Season ") + SeasonNumber
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                    }

                    Label {
                        id: episodeNumber
                        text: qsTr(" Episode ") + EpisodeNumber
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

            // icon-m-certificates
            // icon-m-favorite
            // icon-m-favorite-selected
            // icon-l-check

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
                source: setSource()
                anchors.right: container.right
                anchors.rightMargin: Theme.paddingMedium
                anchors.top: container.top
                anchors.topMargin: Theme.paddingMedium

                MouseArea {
                    id: clickarea
                    onClicked: {
                        Watched === 0 ? Watched = 1 : Watched = 0
                        controller.EpisodeListModel.toggleWatched(ID)
                    }
                    anchors.fill: parent
                }

            }
        }


        VerticalScrollDecorator {
            id: decorator
        }
    }

    EpisodeOverviewPage {
        id: episodeOverview
        anchors.centerIn: parent
        visible: false
    }

    // TODO: how to get this around EpisodeOverviewPage
    //    TouchBlocker {
    //        id: blocker
    //        enabled: false
    //        anchors.fill: listView
    //    }

    ViewPlaceholder {
        enabled: listView.count === 0
        text: "No episodes."
    }
}

