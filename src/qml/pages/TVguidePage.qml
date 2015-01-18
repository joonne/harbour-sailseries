import QtQuick 2.0
import Sailfish.Silica 1.0
import harbour.sailseries.datamodel 1.0

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
            //Channel {id: tv2 }
            //Channel {id: mtv3 }
        }
    }

    function initialize() {

        tv1.initialize("Yle TV1");
        //tv2.initialize("Yle TV2");
        //mtv3.initialize("MTV3");

    }

    Component.onCompleted: {
        console.log("TVGuidePage valmis");
        //timer.start();
        initialize()

    }
}
