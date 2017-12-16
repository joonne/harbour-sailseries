import QtQuick 2.0
import Sailfish.Silica 1.0

Image {
    id: banner

    property string bannerPath
    property int sourceWidth

    source: "http://thetvdb.com/banners/" + bannerPath
    sourceSize.width: sourceWidth
    anchors.left: parent.left
    anchors.leftMargin: {
        return (sourceSize.width - width) / 2
    }

    onStatusChanged: {
        if (status === Image.Error || status === Image.Null) {
            source = "qrc:///images/series-banner-fallback.jpg";
        }
    }
}
