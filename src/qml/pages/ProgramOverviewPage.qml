import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components"

Page {
    id: programoverviewpage

    // these are pushed from previous page with the pageStack
    property string programName
    property string programOverview
    property string guestStars
    property string start
    property string end


    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            anchors.top: parent.top
            anchors.topMargin: Theme.paddingMedium

            PageHeader {
                id: header
                title: programName
            }

            Item {
                id: padding
                height: Theme.paddingLarge
                width: programoverviewpage.width
            }

            TextField {
                id: timeField
                text: start + " - " + end
                readOnly: true
                color: Theme.secondaryColor
                label: qsTr("Time")
                font.pixelSize: Theme.fontSizeSmall
                width: programoverviewpage.width

            }

            TextExpander {
                id: expander
                width: programoverviewpage.width
                textContent: programOverview
            }
        }
    }
}

