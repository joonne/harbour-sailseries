import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: mySeriesPage

    Component.onCompleted: {
        controller.SeriesListModel.populateBannerList()
     }

    Component.onDestruction: {
        controller.SeriesListModel.Mode = "default"
    }

    function nextEpisodeDetails(episodeName,daysTo,status) {

        if(status === "Ended") {
            return "This show has ended";
        } else if(daysTo === "today") {
            return episodeName + " airs today";
        } else if(daysTo === "tomorrow") {
            return episodeName + " airs tomorrow";
        } else if(episodeName.length !== 0) {
            return episodeName + " airs in " + daysTo + " days";
        } else if(episodeName.length === 0 && daysTo !== "unknown") {
            return "Next episode airs in " + daysTo + " days";
        } else {
            return "No information about next episode";
        }
    }

    SilicaListView {
        id: listView
        anchors.fill: parent
        model: controller.SeriesListModel.SeriesList

        header: PageHeader {
            id: header
            title: "My Series"
        }

        delegate: ListItem {
            id: listItem
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
                height: banner.height + seriesName.height + nextEpisode.height + Theme.paddingLarge
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingMedium
                anchors.right: parent.right
                anchors.rightMargin: Theme.paddingMedium
                onClicked: {
                    controller.SeriesListModel.Mode = "mySeries"
                    controller.SeriesListModel.selectSeries(index)
                    pageStack.push(Qt.resolvedUrl("SeriesViewPage.qml"),{seriesID: ID})
                }
            }

            Column {
                spacing: Theme.paddingSmall

                Image {
                    id: banner
                    source: "http://thetvdb.com/banners/" + Banner
                    sourceSize.width: container.width
                    anchors.left: parent.left
                    anchors.leftMargin: Theme.paddingMedium

                }

                Label {
                    id: seriesName
                    text: SeriesName
                    anchors.left: parent.left
                    anchors.leftMargin: 2 * Theme.paddingMedium
                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.primaryColor

                }

                Label {
                    id: nextEpisode
                    text: nextEpisodeDetails(NextEpisodeName,DaysToNextEpisode,Status)
                    anchors.left: parent.left
                    anchors.leftMargin: 2 * Theme.paddingMedium
                    font.pixelSize: Theme.fontSizeExtraSmall
                    color: Theme.primaryColor
                }
            }
        }

        VerticalScrollDecorator {
            id: decorator
        }

        ViewPlaceholder {
            enabled: listView.count === 0
            text: "Search for series first."
            anchors.centerIn: listView

        }
    }

}
