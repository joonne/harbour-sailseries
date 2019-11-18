import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components"

Page {
    id: infopage

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        PullDownMenu {
            busy: engine.SearchModel.Loading

            MenuItem {
                text: "IMDB"
                onClicked: Qt.openUrlExternally("http://www.imdb.com/title/" + engine.SearchModel.IMDB_ID)
            }

            MenuItem {
                visible: !engine.SearchModel.Added
                text: qsTr("Add to my series")
                onClicked: {
                    engine.SearchModel.getAll(engine.SearchModel.ID)
                    appWindow.notificationhandler.publish("summary", "body", "previewSummary", "previewBody")
                }
            }

            MenuItem {
                enabled: !engine.SearchModel.Added
                visible: engine.SearchModel.Added
                text: qsTr("Already added")
            }
        }

        Column {
            id: column
            spacing: Theme.paddingSmall

            PageHeader {
                id: header
                title: engine.SearchModel.SeriesName
                description: engine.SearchModel.Network
            }

            SeriesBanner {
                id: banner
                bannerPath: engine.SearchModel.Banner
                sourceWidth: infopage.width - Theme.paddingMedium * 2
            }

            TextArea {
                label: qsTr("Overview")
                width: infopage.width
                text:  engine.SearchModel.Overview
                readOnly: true
            }
        }
    }
}
