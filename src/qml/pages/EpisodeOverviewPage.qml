import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components"

Page {
    id: episodeoverviewpage

    function setWidth() {
        if(banner.width === 0) {
            return 400
        } else {
            return banner.width + 2 * Theme.paddingMedium
        }
    }

    // small error is better than unnecessary http-request. :D
    function setSource() {
        if(episodeBanner.length === 0) {
            return null;
        } else {
            return "http://thetvdb.com/banners/" + episodeBanner;
        }
    }

    function process(string) {

        if(string.charAt(0) === "|" && string.charAt(string.length - 1) === "|") {
            var newstring = string.split("|").join(", ")
            return newstring.substr(2,(newstring.length - 4))
        } else if(string.charAt(0) === "|") {
            return string.split("|").join(", ").substr(2)
        } else {
            return string.split("|").join(", ")
        }
    }

    property string episodeBanner
    property string episodeOverview
    property string episodeName
    property string guestStars
    property string writer

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            anchors.top: parent.top
            anchors.topMargin: Theme.paddingMedium

            PageHeader {
                id: header
                title: episodeName
            }

            Image {
                id: banner
                source: setSource()
                anchors.left: parent.left
                anchors.leftMargin: (episodeoverviewpage.width - banner.width) / 2
            }

            Item {
                id: padding
                height: Theme.paddingLarge
                width: episodeoverviewpage.width
            }

//            TextField {
//                id: name
//                text: episodeName
//                readOnly: true
//                color: Theme.secondaryColor
//                label: qsTr("Name")
//                font.pixelSize: Theme.fontSizeSmall
//                width: episodeoverviewpage.width

//            }

            TextExpander {
                id: expander
                width: episodeoverviewpage.width
                textContent: episodeOverview
            }

            TextField {
                id: writerField
                width: episodeoverviewpage.width
                text: process(writer)
                label: qsTr("Writer")
                color: Theme.secondaryColor
                readOnly: true
            }

            TextArea {
                id: guestStarsField
                width: episodeoverviewpage.width
                text: process(guestStars)
//                    guestStars.charAt(0) === "|" ?
//                          guestStars.split("|").join(", ").substr(2) :
//                          guestStars.split("|").join(", ")
                label: qsTr("Guest Stars")
                color: Theme.secondaryColor
                readOnly: true
            }
        }
    }
}

