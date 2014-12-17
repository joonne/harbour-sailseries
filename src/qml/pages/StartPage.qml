import QtQuick 2.0
import Sailfish.Silica 1.0
import DataModel 1.0

Page {
    id: startpage

    Component.onCompleted: console.log(pageStack.currentPage)

    SilicaFlickable {
        anchors.fill: parent

        PageHeader {
            id: header
            title: "SailSeries"
        }

        PullDownMenu {

            MenuItem {
                text: "Search for a series"
                font.pixelSize: Theme.fontSizeSmall
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("SearchPage.qml"))
                }
            }

            MenuItem {
                text: "My series"
                font.pixelSize: Theme.fontSizeSmall
                onClicked: {
                    LISTMODEL.Mode = "mySeries"
                    pageStack.push(Qt.resolvedUrl("MySeriesPage.qml"))
                }
            }

            //            MenuItem {
            //                text: "test"
            //                font.pixelSize: Theme.fontSizeSmall
            //                onClicked: pageStack.push(Qt.resolvedUrl("test.qml"))
            //            }

            //            MenuItem {
            //                text: "TV-Guide"
            //                onClicked: pageStack.push(Qt.resolvedUrl("TVguidePage.qml"))
            //            }

        }


        SilicaListView {
            id: listView
            height: 0.5 * startpage.height
            width: startpage.width
            anchors.top: header.bottom
            model: LISTMODEL.seriesList


            header: PageHeader {
                title: "Today"
            }

            delegate: ListItem {
                id: item
                contentHeight: column.height * 1.1
                contentWidth: listView.width

                Column {
                    id: column
                    x: Theme.paddingLarge

                    Label {
                        id: seriesName
                        text: SeriesName.length === 0 ? text = "SeriesName" : text = SeriesName
                        color: Theme.primaryColor
                    }

                    Label {
                        id: network
                        text: Network.length === 0 ? text = "Network" : text = Network
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                    }

                    Label {
                        id: time
                        text: AirsTime.length === 0 ? text = "AirsTime" : text = AirsTime
                        font.pixelSize: Theme.fontSizeSmall
                        color: Theme.secondaryColor
                    }
                }
            }

            ViewPlaceholder {
                enabled: listView.count === 0
                text: "Nothing airs today."
                anchors.centerIn: listView

            }
        }
    }
}

