import QtQuick 2.0
import org.nemomobile.notifications 1.0

Item {
    property string asd: "asd"

    Notification {
        id: notification
        category: "x-nemo.example"
    }


    function publish() {

        notification.summary = "asd"
        notification.body = "asd"
        notification.previewSummary = "asd"
        notification.previewBody = "asd"
        notification.replacesId = 0
        notification.timestamp = new Date("yyyy-MM-dd hh:mm:ss")
        notification.publish()

    }
}

