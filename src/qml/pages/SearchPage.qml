import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailseries.datamodel 1.0

Page {
    id: searchpage

    Component.onDestruction: {
        DATAMODEL.SearchModel.clearList()
    }

    TextField {
        id: seriesSearch
        width: searchpage.width
        anchors.top: searchpage.top
        anchors.topMargin: 100
        placeholderText: "Search for a series"
        EnterKey.onClicked: {
            DATAMODEL.SearchModel.searchSeries(text)
            focus = false
        }
    }

    SilicaListView {
        id: listView
        anchors.top: seriesSearch.bottom
        height: 800
        width: 540
        model: DATAMODEL.SearchModel.SearchModel

        delegate: ListItem {
            id: listItem
            contentHeight: Theme.itemSizeSmall
            contentWidth: listView.width
            onClicked: {
                DATAMODEL.SearchModel.selectSeries(index)
                DATAMODEL.SearchModel.checkIfAdded(ID,SeriesName)
                pageStack.push(Qt.resolvedUrl("SeriesInfoPage.qml"))
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
