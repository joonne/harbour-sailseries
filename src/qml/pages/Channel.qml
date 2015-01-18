import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailseries.datamodel 1.0

Item {

    Component.onCompleted: {
        console.log("kanava valmis");
    }

    height: channelView.height;
    width: channelView.width;

//    property ProgramListModel programlist
//    property ProgramData programData

    function initialize(channel) {
        console.log("kutsutaan populointia");
        //dataModel.setDesiredChannel(channel);
        listview.model = DATAMODEL.ProgramListModel
        console.log("ProgramListModel " + DATAMODEL.ProgramListModel)
        //pageheader.title = programlist.channel
    }

    SilicaListView {
        id: listview

        header: Component {
            PageHeader {
                id: pageheader
                title: "kanava"
            }
        }

        anchors.fill: parent

        delegate: ListItem {

            width: parent.width

            Column {

                Label {
                    text: programName
                }

                Label {
                    text: time

                }
            }
        }

        ViewPlaceholder {
            enabled: listview.count === 0
            text: "No programs today."
        }
    }
}

