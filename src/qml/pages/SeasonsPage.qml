import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: seasonspage

    Component.onCompleted: {
        engine.SeasonListModel.getSeasons(engine.SeriesListModel.ID)
    }

    SilicaListView {
        id: listView
        anchors.fill: parent
        model: engine.SeasonListModel.SeasonList

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
                height: 11 * Theme.paddingLarge
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingMedium
                anchors.right: parent.right
                anchors.rightMargin: Theme.paddingMedium
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("EpisodesPage.qml"), {
                                       seriesID: engine.SeriesListModel.ID,
                                       seasonNumber: index + 1
                                   });
                }
            }

            Image {
                id: banner
                source: "http://thetvdb.com/banners/" + SeasonBanner
                sourceSize.height: 10 * Theme.paddingLarge
                anchors.left: background.left
                anchors.leftMargin: banner.width === 0 ? 0 : Theme.paddingMedium
                anchors.top: parent.top
                anchors.topMargin: (background.height - banner.height) / 2
                onWidthChanged: {
                    if (banner.width > listItem.width) {
                        banner.width = 0;
                    }
                }

                onStatusChanged: {
                    var fallback = "qrc:///images/season-banner-fallback.jpg";
                    if (status === Image.Error || status === Image.Null) {
                        source = fallback;
                    }
                }
            }

            Row {
                id: upperrow
                anchors.top: background.top
                anchors.topMargin: 2 * Theme.paddingLarge
                anchors.left: banner.right
                anchors.leftMargin: (background.width - banner.width - seasonNumber.width) / 2

                Label {
                    id: seasonNumber
                    text: qsTr("Season") + " " + (index + 1)
                    font.pixelSize: Theme.fontSizeLarge
                }
            }

            Row {
                id: middlerow
                anchors.left: banner.right
                anchors.leftMargin: (background.width - banner.width - middlerow.width) / 2
                anchors.top: upperrow.bottom
                anchors.topMargin: Theme.paddingLarge

                Label {
                    text: qsTr("Watched")
                    font.pixelSize: Theme.fontSizeExtraSmall
                }

                Label {
                    text: " "
                    font.pixelSize: Theme.fontSizeExtraSmall
                }

                Label {
                    id: watchedCount
                    text: WatchedCount
                    font.pixelSize: Theme.fontSizeExtraSmall
                }

                Label {
                    text: " "
                    font.pixelSize: Theme.fontSizeExtraSmall
                }

                Label {
                    text: "/"
                    font.pixelSize: Theme.fontSizeExtraSmall
                }

                Label {
                    text: " "
                    font.pixelSize: Theme.fontSizeExtraSmall
                }

                Label {
                    id: totalCount
                    text: TotalCount
                    font.pixelSize: Theme.fontSizeExtraSmall
                }

                Label {
                    text: " "
                    font.pixelSize: Theme.fontSizeExtraSmall
                }

                Label {
                    text: qsTr("episodes")
                    font.pixelSize: Theme.fontSizeExtraSmall
                }
            }
        }

        VerticalScrollDecorator {
            id: decorator
        }
    }
}
