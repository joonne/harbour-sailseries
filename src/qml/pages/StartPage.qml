import QtQuick 2.0
import Sailfish.Silica 1.0
import DataModel 1.0

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
                font.pixelSize: Theme.fontSizeMedium
                onClicked: pageStack.push(Qt.resolvedUrl("SearchPage.qml"))
            }

            MenuItem {
                text: "My series (nothing yet)"
                font.pixelSize: Theme.fontSizeMedium
            }

//            MenuItem {
//                text: "TV-Guide"
//                onClicked: pageStack.push(Qt.resolvedUrl("TVguidePage.qml"))
//            }

        }
    }

}
