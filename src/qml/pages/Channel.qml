import QtQuick 2.0
import Sailfish.Silica 1.0
import "parser.js" as Parser

Item {

    property string channelName;

    height: channelView.height;
    width: channelView.width;

    function initialize(channel) {
        channelName = channel;
        Parser.load(channel);
    }

    ListModel {  id:listModel }

    SilicaListView {
        id: listview

        header: Component {
            PageHeader {
                id: pageheader
                title: channelName
            }
        }

        anchors.fill: parent
        model: listModel
        delegate: ListItem {

            width: parent.width

            Column {
                x: Theme.paddingLarge
                width: parent.width - Theme.paddingLarge

                Label {
                    color: Theme.primaryColor
                    text: name
                }

                Label {
                    font.pixelSize: Theme.fontSizeSmall
                    color: Theme.secondaryColor
                    text: start

                }
            }
        }

        VerticalScrollDecorator {
            id: decorator
        }

        ViewPlaceholder {
            enabled: listview.count === 0
            text: "No programs today."
        }
    }
}

