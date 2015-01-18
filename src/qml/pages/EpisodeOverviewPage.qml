import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailseries.datamodel 1.0

Rectangle {

    function setWidth() {
        if(banner.width === 0) {
            return 400
        } else {
            return banner.width + 2 * Theme.paddingMedium
        }
    }

    id: container
    width: setWidth()
    height: banner.height + overview.height + name.height + button.height + 10
    radius: 5
    color: Theme.rgba(Theme.highlightBackgroundColor, 1.0)

    property string episodeBanner
    property string episodeOverview
    property string episodeName

    BackgroundItem {
        anchors.fill: parent

        Column {

            anchors.top: parent.top
            anchors.topMargin: Theme.paddingMedium

            Image {
                id: banner
                source: "http://thetvdb.com/banners/" + episodeBanner
                anchors.left: parent.left
                anchors.leftMargin: Theme.paddingMedium
            }

            TextField {
                id: name
                text: episodeName
                readOnly: true
                color: "black"
                label: "Name"
                font.pixelSize: Theme.fontSizeSmall
                width: container.width

            }

            TextArea {
                id: overview
                readOnly: true
                text: episodeOverview
                width: container.width
                color: "black"
                height: 250
                font.pixelSize: Theme.fontSizeSmall
                label: "Overview"
            }

            Button {
                id: button
                onClicked: {
                    container.visible = false
                }
                text: "Close"
                color: "black"
                anchors.left: parent.left
                anchors.leftMargin: (container.width - button.width) / 2
            }
        }
    }
}

