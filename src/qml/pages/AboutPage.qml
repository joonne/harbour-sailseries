import QtQuick 2.0
import Sailfish.Silica 1.0
import org.nemomobile.notifications 1.0

import '../components'

Page {
    id: aboutpage

    readonly property string _APP_VERSION: appVersion
    readonly property string _APP_BUILD_NUMBER: appBuildNum

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width: parent.width

            PageHeader {
                title: qsTr("About")
            }

            Image {
                id: icon
                source: "image://theme/harbour-sailseries"
                anchors.left: parent.left
                anchors.leftMargin: (aboutpage.width - icon.width) / 2
            }

            HorizontalSeparator { }

            Label {
                id: appname
                text: "SailSeries"
                anchors.left: parent.left
                anchors.leftMargin: (aboutpage.width - appname.width) / 2
                color: Theme.primaryColor
            }

            Label {
                id: version
                text: qsTr("Version %1.%2").arg(_APP_VERSION).arg(_APP_BUILD_NUMBER)
                anchors.left: parent.left
                anchors.leftMargin: (aboutpage.width - version.width) / 2
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeExtraSmall
            }

            HorizontalSeparator { }

            SectionHeader {
                text: qsTr("General")
            }

            TextArea {
                width: aboutpage.width
                readOnly: true
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: qsTr("SailSeries is a tv-series tracking application for SailfisOS. It aims to be a good looking and simple app that can help you to stay on track while following your favourite series.")
            }

            SectionHeader {
                text: qsTr("Creator")
            }

            TextArea {
                id: creator
                width: aboutpage.width
                readOnly: true
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: qsTr("Jonne Pihlanen, 'joonne'")
                anchors {
                    left: parent.left
                    leftMargin: (aboutpage.width - creator.width) / 2
                }
            }

            Button {
                id: email
                text: "jonne.pihlanen@gmail.com"
                onClicked: Qt.openUrlExternally("mailto:jonne.pihlanen@gmail.com?subject=SailSeries&body=")
                anchors.left: parent.left
                anchors.leftMargin: (aboutpage.width - email.width) / 2
            }

            HorizontalSeparator { }

            SectionHeader {
                text: qsTr("Contributors")
            }

            Repeater {

                model: ["bonapart", "lunatix"]

                TextField {
                    width: aboutpage.width
                    readOnly: true
                    color: Theme.primaryColor
                    font.pixelSize: Theme.fontSizeSmall
                    text: modelData
                }
            }

            SectionHeader {
                text: qsTr("Licence")
            }

            TextArea {
                width: aboutpage.width
                readOnly: true
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: qsTr("This software is licenced under GPLv2.")
            }

            SectionHeader {
                text: qsTr("Content")
            }

            TextArea {
                width: aboutpage.width
                readOnly: true
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: qsTr("SailSeries utilises the public API at TheTVDB.")
            }

            Button {
                id: thetvdb
                text: "TheTVDB"
                onClicked: Qt.openUrlExternally("http://www.thetvdb.com/")
                anchors.left: parent.left
                anchors.leftMargin: (aboutpage.width - thetvdb.width) / 2
                width: email.width
            }

            SectionHeader {
                text: qsTr("Source Code")
            }

            TextArea {
                width: aboutpage.width
                readOnly: true
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: qsTr("The source code is available at GitHub. Contributions, ideas and bug reports are welcome.")
            }

            Button {
                id: sourcecode
                text: qsTr("Project in GitHub")
                onClicked: Qt.openUrlExternally("https://github.com/joonne/harbour-sailseries")
                anchors.left: parent.left
                anchors.leftMargin: (aboutpage.width - sourcecode.width) / 2
                width: email.width
            }

            SectionHeader {
                text: qsTr("Translations")
            }

            TextArea {
                width: aboutpage.width
                readOnly: true
                color: Theme.primaryColor
                font.pixelSize: Theme.fontSizeSmall
                text: qsTr("SailSeries uses the Transifex translation platform to host the translations.")
            }

            Button {
                id: transifex
                text: qsTr("Help with translations")
                onClicked: Qt.openUrlExternally("https://www.transifex.com/joonne/sailseries/")
                anchors.left: parent.left
                anchors.leftMargin: (aboutpage.width - sourcecode.width) / 2
                width: email.width
            }

            Button {
                Notification {
                    id: notification
                    category: "x-nemo.example"
                    summary: "Notification summary"
                    body: "Notification body"
                    onClicked: console.log("Clicked")
                }
                text: "Application notification" + (notification.replacesId ? " ID:" + notification.replacesId : "")
                onClicked: notification.publish()
            }

            HorizontalSeparator { }
        }

        VerticalScrollDecorator { }
    }
}
