import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: mySeriesPage

    Component.onDestruction: {
        controller.SeriesListModel.Mode = "default"
    }

    function nextEpisodeDetails(episodeName, daysTo, status) {

        if(status === "Ended") {
            return qsTr("This show has ended");
        } else if(daysTo === "today") {
            return qsTr("%1 airs tonight").arg(episodeName);
        } else if(daysTo === "tomorrow") {
            return qsTr("%1 airs tomorrow").arg(episodeName);
        } else if(episodeName.length !== 0) {
            return qsTr("%1 airs in %2 days").arg(episodeName).arg(daysTo);
        } else if(episodeName.length === 0 && daysTo.length !== 0) {
            return qsTr("Next episode airs in %1 days").arg(daysTo);
        } else {
            return qsTr("No information about next episode");
        }
    }

    SilicaListView {
        id: listView
        anchors.fill: parent
        model: controller.SeriesListModel.SeriesList

        header: PageHeader {
            id: header
            title: qsTr("My Series")
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
                    pageStack.push(Qt.resolvedUrl("SeriesPage.qml"),{seriesID: ID})
                }
            }

            Column {
                id: column
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
                    text: nextEpisodeDetails(NextEpisodeName, DaysToNextEpisode, Status)
                    anchors.left: parent.left
                    anchors.leftMargin: 2 * Theme.paddingMedium
                    font.pixelSize: Theme.fontSizeTiny
                    color: Theme.primaryColor
                }
            }
        }

        VerticalScrollDecorator {
            id: decorator
        }

        ViewPlaceholder {
            visible: listView.count === 0
            text: qsTr("Search for series first.")
            anchors.centerIn: listView
        }

        BusyIndicator {
            size: BusyIndicatorSize.Large
            anchors.centerIn: parent
            running: controller.SeriesListModel.Loading ? true : false
            visible: false
        }
    }
}
