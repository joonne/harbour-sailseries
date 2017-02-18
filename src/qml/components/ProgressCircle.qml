import QtQuick 2.0
import Sailfish.Silica 1.0

ProgressCircleBase {

    id: progress

    property double value: 0
    property double size: Theme.itemSizeExtraLarge

    height: size
    width: size
    implicitWidth: Theme.itemSizeHuge

    borderWidth: Math.round(Theme.paddingLarge / 3)
    backgroundColor: Theme.rgba(Theme.highlightColor, 0.2)

    progressColor: Theme.rgba(Theme.highlightColor, 0.4)
    progressValue: value

    Text {
        anchors.centerIn: progress
        text: qsTr("%1 %").arg(Math.round(value * 100))
        font.pixelSize: Theme.fontSizeSmall
        color: Theme.secondaryColor
    }
}
