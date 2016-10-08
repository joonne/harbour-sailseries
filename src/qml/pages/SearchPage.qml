import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: searchpage

    Component.onDestruction: {
        engine.SearchModel.clearList()
    }

    Timer {
        id: textDebounce
        interval: 1000
        onTriggered: {
            engine.SearchModel.searchSeries(seriesSearch.text)
        }
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
            engine.SearchModel.searchSeries(text)
            focus = false
        }

        onTextChanged: {
            if (text.length > 0) {
                textDebounce.start()
            } else {
                engine.SearchModel.clearList()
            }
        }
    }

    SilicaListView {
        id: listView
        anchors.top: seriesSearch.bottom
        height: (searchpage.height - seriesSearch.height - Theme.paddingLarge)
        width: searchpage.width
        clip: true
        model: engine.SearchModel.SearchModel

        delegate: ListItem {
            id: listItem
            contentHeight: Theme.itemSizeSmall
            contentWidth: listView.width
            onClicked: {
                engine.SearchModel.selectSeries(index)
                engine.SearchModel.checkIfAdded(ID,SeriesName)
                pageStack.push(Qt.resolvedUrl("SeriesInfoPage.qml"))
            }

            Column {
                x: Theme.paddingLarge
                Label {
                    id: seriesName
                    text: SeriesName
                    width: listItem.width - (2 * Theme.paddingLarge)
                    truncationMode: TruncationMode.Fade
                    color: Theme.primaryColor
                }
                Label {
                    id: network
                    text: Network
                    font.pixelSize: Theme.fontSizeSmall
                    width: parent.width - Theme.paddingLarge
                    truncationMode: TruncationMode.Fade
                    color: Theme.secondaryColor
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
