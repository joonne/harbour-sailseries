import QtQuick 2.0
import Sailfish.Silica 1.0
import SeriesModel 1.0

Page {
    id: infopage

    property SeriesListModel listModel

    Component.onCompleted: {
        console.log("SeriesInfoPage on complete " + listModel);
    }

    SilicaFlickable {
        anchors.fill: parent

        Column {

            PageHeader {
                id: header
                title: listModel.SeriesName
            }

            Separator {
                height: Theme.PaddingMedium
                width: infopage.width - Theme.PaddingMedium * 2
                color: Theme.secondaryColor
            }

            TextArea {
                id: seriesname
                width: infopage.width
                height: 500
                text: listModel.Overview
                readOnly: true
                color: Theme.secondaryColor
            }


            Separator {
                height: Theme.PaddingMedium
                width: infopage.width
                color: Theme.secondaryColor - Theme.PaddingMedium * 2

            }

            TextField {
                id: network
                text: listModel.Network
                color: Theme.secondaryColor
                readOnly: true

            }

        }
    }

}
