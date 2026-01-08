import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components"

Page {
    id: searchresultpage

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

            Image {
                source: banner
                width: searchresultpage.width * (9/16)
                anchors.left: parent.left
                anchors.leftMargin: (parent.width - width) / 2
                fillMode: Image.PreserveAspectFit
            }

            TextArea {
                label: qsTr("Overview")
                width: searchresultpage.width
                text:  overview
                readOnly: true
            }

            VerticalScrollDecorator {
                id: decorator
            }
        }
    }
}
