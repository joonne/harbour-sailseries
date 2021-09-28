import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components"

Page {
    id: infopage

    property int id
    property string name
    property string network
    property string banner
    property string overview
    property string imdbId
    property bool isAdded

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            busy: engine.Loading

            MenuItem {
                text: "IMDB"
                onClicked: Qt.openUrlExternally("http://www.imdb.com/title/" + imdbId)
            }

            MenuItem {
                visible: !isAdded
                text: qsTr("Add to my series")
                onClicked: {
                    engine.SearchModel.getAll(id)
                }
            }

            MenuItem {
                enabled: !isAdded
                visible: isAdded
                text: qsTr("Already added")
            }
        }

        Column {
            id: column
            spacing: Theme.paddingMedium

            PageHeader {
                id: header
                title: name
                description: network
            }

            SeriesBanner {
                bannerPath: banner
                sourceWidth: infopage.width - Theme.paddingMedium * 2
            }

            TextArea {
                label: qsTr("Overview")
                width: infopage.width
                text:  overview
                readOnly: true
            }

            VerticalScrollDecorator {
                id: decorator
            }
        }
    }
}
