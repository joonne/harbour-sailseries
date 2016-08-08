import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components"

Page {
    id: episodeoverviewpage

    // prevents http-request if source is not available
    function setSource() {
        if(episodeBanner.length === 0) {
            return null;
        } else {
            return "http://thetvdb.com/banners/" + episodeBanner;
        }
    }

    // processes tubes "|" out of the given string
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

    // these are pushed from previous page with the pageStack
    property string episodeBanner
    property string episodeOverview
    property string episodeName
    property string guestStars
    property string writer
    property string firstAired

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

                onStatusChanged: {
                    var fallback = "qrc:///images/episode-image-fallback.jpg";
                    if (status === Image.Error || status === Image.Null) {
                        source = fallback;
                    }
                }
            }

            Item {
                id: padding
                height: Theme.paddingLarge
                width: episodeoverviewpage.width
            }

            TextField {
                id: firstAiredField
                text: firstAired
                readOnly: true
                color: Theme.secondaryColor
                label: qsTr("Original air date")
                font.pixelSize: Theme.fontSizeSmall
                width: episodeoverviewpage.width

            }

            TextExpander {
                id: expander
                width: episodeoverviewpage.width
                textContent: episodeOverview
            }

            TextArea {
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
                label: qsTr("Guest Stars")
                color: Theme.secondaryColor
                readOnly: true
            }
        }
    }
}

