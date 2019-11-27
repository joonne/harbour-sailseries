import QtQuick 2.0
import Sailfish.Silica 1.0

Image {
    id: banner

    property string bannerPath
    property int sourceWidth

    source: bannerPath.indexOf("banners") === -1 ? "https://thetvdb.com/banners/" + bannerPath : "https://thetvdb.com/" + bannerPath
    sourceSize.width: sourceWidth
    anchors.left: parent.left
    anchors.leftMargin: {
        return sourceSize.width - width <= 0 ? Theme.paddingMedium : ((sourceSize.width - width) / 2 + Theme.paddingMedium)
    }

    onStatusChanged: {
        if (status === Image.Error || status === Image.Null) {
            source = "qrc:///images/series-banner-fallback.jpg";
        }
    }
}
