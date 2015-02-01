import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: aboutpage

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
    }
}
