import QtQuick 2.0
import org.nemomobile.notifications 1.0

Item {

    property variant notifications: []

    Notification {
        id: notification
        category: "x-nemo.example"
        onClicked: activate()
        remoteActions: [ {
                "name": "default",
                "displayName": "Do something",
                "icon": "icon-s-do-it",
                "service": "org.nemomobile.example",
                "path": "/example",
                "iface": "org.nemomobile.example",
                "method": "doSomething",
                "arguments": [ "argument", 1 ]
            } ]
    }

    function activate() {
        appWindow.activate()
    }

    function findNotificationId(seriesName) {
        var id = 0;
        notifications.forEach(function(item) {
            if(item.seriesName === seriesName) {
                id = item.id;
            }
        });
        return id;
    }

    function publish(summary, body, previewSummary, previewBody) {
        var replacesId = findNotificationId(summary);

        notification.summary = summary
        notification.body = body
        notification.previewSummary = previewSummary
        notification.previewBody = previewBody
        notification.replacesId = replacesId
        notification.timestamp = new Date("yyyy-MM-dd hh:mm:ss")
        notification.publish()

        if(replacesId === 0) {
            var temp = notifications;
            temp.push({id: notification.replacesId, seriesName: summary});
            notifications = temp;
        }
    }
}
