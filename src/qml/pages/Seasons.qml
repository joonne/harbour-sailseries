import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: item
    height: seriesView.height;
    width: seriesView.width;

    // HOWTODO: Update the icons when season is marked as watched !

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
            title: qsTr("Seasons")
        }

        delegate: ListItem {
            contentHeight: button.height + Theme.paddingLarge + Theme.paddingLarge

            Column {
                spacing: Theme.paddingLarge
                Button {
                    id: button
                    text: qsTr("Season") + " " + (index + 1)
                    anchors.left: parent.left
                    anchors.leftMargin: (item.width - button.width - Theme.paddingMedium - Theme.paddingMedium) / 2
                    onClicked: {
                        pageStack.push(Qt.resolvedUrl("EpisodesPage.qml"), { seriesID: controller.SeriesListModel.ID, seasonNumber: index + 1});
                    }
                }
            }
        }

        VerticalScrollDecorator {
            id: decorator
        }
    }
}
