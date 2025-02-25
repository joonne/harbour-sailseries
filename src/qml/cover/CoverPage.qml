import QtQuick 2.0
import Sailfish.Silica 1.0

CoverBackground {
    id: cover

    Connections {
        target: engine.SeriesListModel
        onPosterChanged: {
            poster.source = setPosterVisible(aPoster)
        }
    }

    function isDefaultVisibility() {
        return engine.Mode === "default"
    }

    Image {
        id: coverBackgroundImage
        asynchronous: true
        fillMode: Image.PreserveAspectFit
        width: parent.height - Theme.paddingLarge
        height: width
        opacity: 0.15
        source: "qrc:///harbour-sailseries.svg"
        property int sourceDimension: Math.min(width, height) - Theme.paddingMedium
        anchors {
            centerIn: undefined
            verticalCenter: parent.verticalCenter
            bottom: parent.bottom
            bottomMargin: Theme.paddingMedium
            right: parent.right
            rightMargin: Theme.paddingMedium
        }
        sourceSize {
            width: sourceDimension
            height: sourceDimension
        }
        visible: isDefaultVisibility()
    }

    //---------------------------------
    // This is the "m_series" coverPage
    //---------------------------------

    function setPosterVisible(aPoster) {
        if (engine.Mode === "m_series" && aPoster) {
            return aPoster;
        }
    }

    Image {
        id: poster
        anchors.top: parent.top
        width: parent.width
        height: parent.height
        visible: engine.Mode === "m_series"
        opacity: 1.0
    }
}
