import QtQuick 2.0
import Sailfish.Silica 1.0
import SeriesModel 1.0

Page {
    id: searchpage

    SeriesListModel {
        id: listModel

    }

    property SeriesListModel listModel


    TextField {
        id: seriesSearch
        width: searchpage.width
        anchors.top: searchpage.top
        anchors.topMargin: 100
        placeholderText: "Search for a series"
        EnterKey.onClicked: {
            listModel.searchSeries(text)
            //testmodel.searchSeries(text)
            focus = false
        }
    }

    SilicaListView {
        id: listView
        anchors.top: seriesSearch.bottom
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
            onPressed: {
                listModel.selectSeries(listView.currentIndex)
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
                    text: Network
                    font.pixelSize: Theme.fontSizeSmall
                    color: listItem.highlighted ? Theme.highlightColor : Theme.secondaryColor
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
