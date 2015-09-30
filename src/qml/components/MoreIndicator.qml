import QtQuick 2.0
import Sailfish.Silica 1.0

Image {
    source: textContainer.expanded ?
                "image://theme/icon-lock-more" :
                "image://theme/icon-lock-more"
    opacity: textContainer.expanded ? 70 : 20
}
