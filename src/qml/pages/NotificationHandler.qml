import QtQuick 2.0
import org.nemomobile.notifications 1.0

Item {

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

    function publish(summary, body, previewSummary, previewBody) {
        notification.summary = summary
        notification.body = body
        notification.previewSummary = previewSummary
        notification.previewBody = previewBody
        notification.replacesId =
        notification.timestamp = new Date("yyyy-MM-dd hh:mm:ss")
        notification.publish()
    }
}
