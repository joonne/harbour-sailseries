import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: item
    height: seriesView.height;
    width: seriesView.width;

    // TODO: Update the icons when season is marked as watched !

    Component.onCompleted: timer.start()

    // initialization is too fast without this
    Timer {
        id: timer
        interval: 600
        onTriggered: {
            listView.model = controller.EpisodeListModel.getSeasonCount(controller.SeriesListModel.ID)
            listView.visible = true
        }
    }

    SilicaListView {
        id: listView
        anchors.fill: parent
        visible: false

        header: PageHeader {
            id: header
            title: "Seasons"
        }

        delegate: ListItem {
            contentHeight: button.height + Theme.paddingLarge + Theme.paddingLarge

            Column {
                spacing: Theme.paddingLarge
                Button {
                    id: button
                    text: "Season " + (index + 1) + " watched"
                    anchors.left: parent.left
                    anchors.leftMargin: (item.width - button.width - Theme.paddingMedium - Theme.paddingMedium) / 2
                    onClicked: controller.EpisodeListModel.markSeasonWatched(controller.SeriesListModel.ID,index+1)
                }
            }
        }

        VerticalScrollDecorator {
            id: decorator
        }
    }
}
