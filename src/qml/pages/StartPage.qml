import QtQuick 2.0
import Sailfish.Silica 1.0
import SeriesModel 1.0

Page {
    id: startpage

    SilicaFlickable {
        anchors.fill: parent

        PageHeader {
            title: "SailSeries"
        }

        PullDownMenu {

            MenuItem {
                text: "Search for a series"
                onClicked: pageStack.push(Qt.resolvedUrl("SearchPage.qml"))
            }

            MenuItem {
                text: "My series"
            }

        }
    }

}
