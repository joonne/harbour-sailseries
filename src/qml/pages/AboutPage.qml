import QtQuick 2.1
import Sailfish.Silica 1.0

Page {
    id: aboutpage

    property string contributor_qzip: "bonapart"
    property string contributors: ""

    PageHeader {
        id: header
        title: qsTr("About SailSeries")
    }

    Column {
        anchors.top: header.bottom
        anchors.topMargin: Theme.paddingLarge * 3

        Image {
            id: icon
            source: "image://theme/harbour-sailseries"
            anchors.left: parent.left
            anchors.leftMargin: (aboutpage.width - icon.width) / 2
        }

        Item {
            height: Theme.paddingMedium
            width: aboutpage.width
        }

        Label {
            id: appname
            text: "SailSeries"
            anchors.left: parent.left
            anchors.leftMargin: (aboutpage.width - appname.width) / 2
            color: Theme.primaryColor
        }

        Item {
            height: Theme.paddingLarge
            width: aboutpage.width
        }

        TextArea {
            width: aboutpage.width
            readOnly: true
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeSmall
            text: qsTr("Created by Jonne Pihlanen.") + " " +
                  qsTr("This software is licenced under GPLv2.") + " " +
                  qsTr("SailSeries fetches all information from a free API at TheTVDB.")
        }

        Button {
            id: thetvdb
            text: qsTr("TheTVDB")
            onClicked: Qt.openUrlExternally("http://www.thetvdb.com/")
            anchors.left: parent.left
            anchors.leftMargin: (aboutpage.width - thetvdb.width) / 2
            width: email.width
        }

        Item {
            height: Theme.paddingLarge
            width: aboutpage.width
        }

        Button {
            id: sourcecode
            text: qsTr("Source code in GitHub")
            onClicked: Qt.openUrlExternally("https://github.com/joonne/harbour-sailseries")
            anchors.left: parent.left
            anchors.leftMargin: (aboutpage.width - sourcecode.width) / 2
            width: email.width
        }

        Item {
            height: Theme.paddingLarge
            width: aboutpage.width
        }

        Button {
            id: transifex
            text: qsTr("Help with translations")
            onClicked: Qt.openUrlExternally("https://www.transifex.com/joonne/sailseries/")
            anchors.left: parent.left
            anchors.leftMargin: (aboutpage.width - sourcecode.width) / 2
            width: email.width
        }

        Item {
            height: Theme.paddingLarge
            width: aboutpage.width
        }

        Button {
            id: email
            text: qsTr("Send email to developer")
            onClicked: Qt.openUrlExternally("mailto:jonne.pihlanen@gmail.com?subject=SailSeries&body=")
            anchors.left: parent.left
            anchors.leftMargin: (aboutpage.width - email.width) / 2
        }

        Item {
            height: Theme.paddingLarge
            width: aboutpage.width
        }

        TextArea {
            width: aboutpage.width
            readOnly: true
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeSmall
            text: qsTr("Thanks to bonapart for the help with qzip.")

        }

        Item {
            height: Theme.paddingLarge
            width: aboutpage.width
        }
    }
}
