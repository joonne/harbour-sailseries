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
                text: qsTr("Allow SailSeries to publish notifications (experimental)")
                checked: settings.getNotificationPreference()
                onCheckedChanged: {
                    checked ? settings.setNotificationPreference(true) : settings.setNotificationPreference(false);
                }
            }

            SectionHeader { text: qsTr("Content updates") }

            TextSwitch {
                text: qsTr("Allow periodic updates of content")
                checked: settings.getContentUpdatePreference();
                onCheckedChanged: {
                    checked ? settings.setContentUpdatePreference(true) : settings.setContentUpdatePreference(false);
                }
            }
        }
    }
}
