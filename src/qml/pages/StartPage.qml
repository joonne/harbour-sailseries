import QtQuick 2.0
import Sailfish.Silica 1.0
import SeriesModel 1.0

Page {
    id: startpage

    TextField {
        id: seriesSearch
        width: startpage.width
        anchors.top: startpage.top
        anchors.topMargin: 50
        placeholderText: "Search for a series"
        EnterKey.onClicked: {
            model.searchSeries(text)
        }
    }

    SilicaListView {
        id: listView
        anchors.top: seriesSearch.bottom
        height: 800
        model: SeriesListModel.seriesList

        header: PageHeader {
            title: "SailSeries"
        }

        delegate: ListItem {
            id: listItem
            contentHeight: Theme.itemSizeSmall
            contentWidth: listView.width
            Column {
                x: Theme.paddingLarge
                Label {
                    id: seriesName
                    text: SeriesName
                    color: listItem.highlighted ? Theme.highlightColor : Theme.primaryColor
                }
                Label {
                    text: Overview
                    font.pixelSize: Theme.fontSizeSmall
                    color: listItem.highlighted ? Theme.highlightColor : Theme.secondaryColor
                }
            }
        }
    }
}
