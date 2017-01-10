import QtQuick 2.0
import Sailfish.Silica 1.0

import '../components'

Page {

    id: statisticspage

    SilicaFlickable {

        anchors.fill: parent
        contentHeight: column.height

        Column {

            id: column
            width: parent.width

            PageHeader {
                title: qsTr("Statistics")
            }

            SectionHeader {
                text: qsTr("Time spent")
            }

            TextField {
                width: statisticspage.width
                readOnly: true
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: qsTr("%1 minutes").arg(engine.getWatchedEpisodesDuration())
                label: qsTr("Time spent watching")
            }
        }

        VerticalScrollDecorator { }
    }
}
