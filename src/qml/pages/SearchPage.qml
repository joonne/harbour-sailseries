import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: searchpage

    Component.onDestruction: {
        controller.SearchModel.clearList()
    }

    SearchField {
        id: seriesSearch
        width: searchpage.width
        anchors {
            top: searchpage.top
            topMargin: (Theme.paddingLarge * 4)
        }
        placeholderText: qsTr("Search for a series")
        EnterKey.onClicked: {
            controller.SearchModel.searchSeries(text)
            focus = false
        }
    }

    SilicaListView {
        id: listView
        anchors.top: seriesSearch.bottom
        height: (searchpage.height - seriesSearch.height - Theme.paddingLarge)
        width: searchpage.width
        model: controller.SearchModel.SearchModel

        delegate: ListItem {
            id: listItem
            contentHeight: Theme.itemSizeSmall
            contentWidth: listView.width
            onClicked: {
                controller.SearchModel.selectSeries(index)
                controller.SearchModel.checkIfAdded(ID,SeriesName)
                pageStack.push(Qt.resolvedUrl("SeriesInfoPage.qml"))
            }

            Column {
                x: Theme.paddingLarge
                Label {
                    id: seriesName
                    text: SeriesName
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

        VerticalScrollDecorator {
            id: decorator
        }

        ViewPlaceholder {
            enabled: listView.count === 0
            text: qsTr("Here will be stuff when you search for something")
            anchors.centerIn: listView

        }
    }
}
