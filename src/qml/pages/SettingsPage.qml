import QtQuick 2.0
import Sailfish.Silica 1.0

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
                text: qsTr("Periodic updates of content")
                description: qsTr("SailSeries refreshes all content every one hour. Requires app to be open at background.")
                checked: settings.getContentUpdatePreference();
                onCheckedChanged: {
                    checked ? settings.setContentUpdatePreference(true) : settings.setContentUpdatePreference(false);
                }
            }
        }
    }
}
