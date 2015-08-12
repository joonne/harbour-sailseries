import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: aboutpage

    property string contributor_qzip: "bonapart"

    PageHeader {
        title: "About SailSeries"
    }

    Column {
        anchors.centerIn: parent

        TextArea {
            width: aboutpage.width
            readOnly: true
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeSmall
            text: "Created by Jonne Pihlanen\n" +
                  "This software is licenced under GPLv2"
        }

        Button {
            id: sourcecode
            text: "Source code in GitHub"
            onClicked: Qt.openUrlExternally("https://github.com/joonne/harbour-sailseries")
            anchors.left: parent.left
            anchors.leftMargin: (aboutpage.width - sourcecode.width) / 2
        }

        Button {
            id: email
            text: "Send email to developer"
            onClicked: Qt.openUrlExternally("mailto:jonne.pihlanen@gmail.com?subject=SailSeries&body=")
            anchors.left: parent.left
            anchors.leftMargin: (aboutpage.width - email.width) / 2
        }

        TextArea {
            width: aboutpage.width
            readOnly: true
            color: Theme.primaryColor
            font.pixelSize: Theme.fontSizeSmall
            text: "\n Thanks to " + contributor_qzip + " for the help with qzip."

        }
    }
}
