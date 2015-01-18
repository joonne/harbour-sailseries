import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailseries.datamodel 1.0

Item {
    id: item
    height: seriesView.height;
    width: seriesView.width;

    SilicaListView {
        id: listView
        anchors.fill: parent
        model: DATAMODEL.EpisodeListModel.getSeasonCount(DATAMODEL.SeriesListModel.ID)

        header: PageHeader {
            title: "Seasons"
        }

        delegate: ListItem {
            contentHeight: button.height + Theme.paddingLarge + Theme.paddingLarge

            Column {
                spacing: Theme.paddingLarge
                Button {
                    id: button
                    text: "mark season " + (index + 1) + " as watched."
                    anchors.left: parent.left
                    anchors.leftMargin: (item.width - button.width) / 2
                    onClicked: DATAMODEL.EpisodeListModel.markSeasonWatched(DATAMODEL.SeriesListModel.ID,index+1)
                }
            }
        }
    }
}
