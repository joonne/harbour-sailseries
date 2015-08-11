import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
    id: tvguidepage

    Timer {
        id: timer
        interval: 5000; running: false; repeat: false
        onTriggered: initialize();
       }

    SlideshowView {
        id: channelView
        width: tvguidepage.width
        height: tvguidepage.height
        itemWidth: width

        model: VisualItemModel {
            id: channels
            Channel {id: tv1 }
            Channel {id: tv2 }
            Channel {id: mtv3 }
            Channel {id: nelonen }
            Channel {id: subtv }
        }
    }

    function initialize() {

        tv1.initialize("yle1");
        tv2.initialize("yle2");
        mtv3.initialize("mtv3");
        nelonen.initialize("nelonen");
        subtv.initialize("subtv");
    }

    Component.onCompleted: {
        console.log("TVGuidePage valmis");
        initialize()

    }
}
