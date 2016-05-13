import QtQuick 2.0
import org.nemomobile.notifications 1.0

Timer {
    id: timer

    Notification {
        id: notification
        category: "x-nemo.example"
        summary: "Game Of Thrones"
        body: "21:00"
        onClicked: console.log("Clicked")
    }

    interval: 10000
    repeat: true
    onTriggered: notification.publish()

    function toggle(state) {
        if(state === true) {
            start();
        } else {
            stop()
        }
    }
}
