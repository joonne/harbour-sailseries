import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: item
    height: seriesView.height;
    width: seriesView.width;

    Component.onCompleted: {
        controller.SeasonListModel.populateSeasonList(controller.SeriesListModel.ID)
        timer.start()
    }

    // initialization is too fast without this
    Timer {
        id: timer
        interval: 600
        onTriggered: {
            listView.model = controller.SeasonListModel.SeasonList
            listView.visible = true
        }
    }

    SilicaListView {
        id: listView
        anchors.fill: parent
        visible: false

        header: PageHeader {
            id: header
            title: qsTr("Seasons")
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
                height: banner.height + row.height + 10
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingMedium
                anchors.right: parent.right
                anchors.rightMargin: Theme.paddingMedium
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("EpisodesPage.qml"), { seriesID: controller.SeriesListModel.ID, seasonNumber: index + 1});
                }
            }

            Column {
                spacing: Theme.paddingLarge

                Image {
                    id: banner
                    source: "http://thetvdb.com/banners/" + SeasonBanner
                    sourceSize.height: 10 * Theme.paddingLarge
                }

                Row {
                    id: row
                    visible: false

                    Label {
                        id: seasonNumber
                        text: qsTr("Season") + " " + (index + 1)
                    }

                    Label {
                        text: " "
                    }

                    Label {
                        id: watchedCount
                        text: WatchedCount
                    }

                    Label {
                        text: " / "
                    }

                    Label {
                        id: totalCount
                        text: TotalCount
                    }
                }
            }
        }

        VerticalScrollDecorator {
            id: decorator
        }
    }
}
