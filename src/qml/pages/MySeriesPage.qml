import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailseries.datamodel 1.0

Page {
    id: mySeriesPage

    Component.onCompleted: {
        DATAMODEL.SeriesListModel.populateBannerList()
     }

    Component.onDestruction: {
        DATAMODEL.SeriesListModel.Mode = "default"
    }

    SilicaListView {
        id: listView
        anchors.fill: parent
        model: DATAMODEL.SeriesListModel.SeriesList

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
                height: banner.height + seriesName.height + 10
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingMedium
                anchors.right: parent.right
                anchors.rightMargin: Theme.paddingMedium
                onClicked: {
                    console.log(ID)
                    DATAMODEL.SeriesListModel.selectSeries(index)
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
            }
        }

        ViewPlaceholder {
            enabled: listView.count === 0
            text: "Search for series first."
            anchors.centerIn: listView

        }
    }

}
