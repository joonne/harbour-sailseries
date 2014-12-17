import QtQuick 2.0
import Sailfish.Silica 1.0
import DataModel 1.0

Page {

    SeriesListModel {
        id: listModel
    }

    SilicaListView {
        id: listView
        height: 800
        width: 540
        model: listModel.seriesList

        header: PageHeader {
            title: "SailSeries"
        }

        delegate: ListItem {
            id: listItem
            contentHeight: Theme.itemSizeSmall
            contentWidth: listView.width
            onClicked: {
                listModel.selectSeries(index)
                pageStack.push(Qt.resolvedUrl("SeriesInfoPage.qml"), {"listModel":listModel})
            }

            Column {
                x: Theme.paddingLarge
                Label {
                    id: seriesName
                    text: SeriesName.length === 0 ? text = "toimii" : text = SeriesName
                    color: listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                }
                Label {
                    id: network
                    text: Banner
                    font.pixelSize: Theme.fontSizeSmall
                    color: listItem.highlighted ? Theme.highlightColor : Theme.secondaryColor
                }

                Image {
                    id: banner
                    source: "http://thetvdb.com/banners/" + Banner
                    sourceSize.width: 540
                }
            }
        }

        ViewPlaceholder {
            enabled: listView.count === 0
            text: "Here will be stuff when you search for something."
            anchors.centerIn: listView

        }
    }

}
