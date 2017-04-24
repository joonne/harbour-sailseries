import QtQuick 2.0
import Sailfish.Silica 1.0

import "../components"

Page {
    id: settingsPage

    SilicaFlickable {
        anchors.fill: parent
        contentHeight: column.height

        Column {
            id: column
            width: parent.width

            PageHeader {
                title: qsTr("Settings")
            }

            SectionHeader { text: qsTr("Notifications") }

            TextSwitch {
                text: qsTr("Publish notifications (experimental)")
                description: qsTr("SailSeries publishes notifications on app startup and when updating/adding series if an episode airs today.")
                checked: settings.getNotificationPreference()
                onCheckedChanged: {
                    checked ? settings.setNotificationPreference(true) : settings.setNotificationPreference(false);
                }
            }

            SectionHeader { text: qsTr("Content updates") }

            TextSwitch {
                text: qsTr("Periodic updates of content (experimental)")
                description: qsTr("SailSeries refreshes local content every one hour. Requires app to be open at background. Might cause a small hickup when content is being updated.")
                checked: settings.getContentUpdatePreference();
                onCheckedChanged: {
                    checked ? settings.setContentUpdatePreference(true) : settings.setContentUpdatePreference(false);
                }
            }

            TextSwitch {
                text: qsTr("Update content of ended series")
                description: qsTr("SailSeries will update also the content of ended series when this is applied.")
                checked: settings.getUpdateEndedSeriesPreference();
                onCheckedChanged: {
                    checked ? settings.setUpdateEndedSeriesPreference(true) : settings.setUpdateEndedSeriesPreference(false);
                }
            }

            SectionHeader { text: qsTr("Maintenance") }

            HorizontalSeparator { }

            HorizontalSeparator { }

            Button {
                text: qsTr("Remove duplicate episodes")
                anchors {
                    left: parent.left
                    leftMargin: (parent.width - width) / 2
                }
                onClicked: remorse.execute("Removing duplicates", function() { engine.deleteDuplicateEpisodes() } )
            }

            RemorsePopup { id: remorse }
        }
    }
}
