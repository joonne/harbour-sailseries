import QtQuick 2.0
import Sailfish.Silica 1.0
import "pages"
import "cover"
import "components"

ApplicationWindow {
    id: appWindow

    initialPage: StartPage { }
    cover: CoverPage { }

    NotificationHandler {
        id: notificationhandler
    }

    Timer {
        id: updateTimer
        interval: 30000
        repeat: true
        onTriggered: engine.updateModels()
        Component.onCompleted: start()
    }
}


