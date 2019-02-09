import QtQuick 2.2
import Nemo.Notifications 1.0

Item {
    id: item

    property variant notifications: []

    Notification {
        id: notification
        category: "x-nemo.example"
        onClicked: activate()
    }

    function activate() {
        appWindow.activate()
    }

    function findNotificationId(seriesName) {
        var notification = notifications.filter(function(item) { return item.seriesName === seriesName; });
        return (notification && notification.id) || 0;
    }

    function clearNotifications() {
        notifications = [];
    }

    function removeNotification(seriesName) {
        notifications = notifications.filter(function(item) {
            return item.seriesName !== seriesName;
        });
    }

    function publish(summary, body, previewSummary, previewBody) {
        var replacesId = findNotificationId(summary);

        if (replacesId === 0) {

            notification.summary = summary
            notification.body = body
            notification.previewSummary = previewSummary
            notification.previewBody = previewBody
            notification.replacesId = replacesId
            notification.timestamp = new Date("yyyy-MM-dd hh:mm:ss")
            notification.publish()

            var temp = notifications;
            temp.push({id: notification.replacesId, seriesName: summary});
            temp.push(notification);
            notifications = temp;
        }
    }
}
