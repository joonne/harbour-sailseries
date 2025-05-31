import QtQuick 2.0
import Sailfish.Silica 1.0

Image {
    id: banner

    property string bannerPath
    property int bannerWidth

    source: bannerPath
    width: bannerWidth
    anchors.left: parent.left
    anchors.leftMargin: Theme.paddingMedium

    onStatusChanged: {
        if (status === Image.Error || status === Image.Null) {
            source = "qrc:///images/series-banner-fallback.jpg";
        }
    }
}
