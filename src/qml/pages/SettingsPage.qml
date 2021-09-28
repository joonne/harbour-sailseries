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

            SectionHeader { text: qsTr("Content updates") }

            TextSwitch {
                text: qsTr("Update content of ended series")
                description: qsTr("SailSeries will update also the content of ended series when this is applied.")
                checked: settings.getUpdateEndedSeriesPreference();
                onCheckedChanged: {
                    checked ? settings.setUpdateEndedSeriesPreference(true) : settings.setUpdateEndedSeriesPreference(false);
                }
            }
        }
    }
}
