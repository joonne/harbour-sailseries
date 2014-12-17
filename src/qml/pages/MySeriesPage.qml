import QtQuick 2.0
import Sailfish.Silica 1.0
import DataModel 1.0

Page {
    id: mySeriesPage

    Component.onCompleted: {
        LISTMODEL.populateBannerList()
     }

    Component.onDestruction: {
        LISTMODEL.Mode = "default"
        LISTMODEL.populateTodayList()
    }

    PageHeader {
        id: header
        title: "My Series"
    }

    SilicaListView {
        id: listView
        height: 1000
        anchors.top: header.bottom
        width: mySeriesPage.width
        model: LISTMODEL.seriesList

        delegate: ListItem {
            id: listItem
            contentHeight: Theme.itemSizeExtraLarge
            contentWidth: listView.width
            onClicked: {
            }

            Column {
                Image {
                    id: banner
                    source: "http://thetvdb.com/banners/" + Banner
                    sourceSize.width: (0.9 * mySeriesPage.width)

                }

                Label {
                    id: seriesName
                    text: "name"

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
