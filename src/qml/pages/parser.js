function load(channel) {

    listModel.clear();
    var xhr = new XMLHttpRequest();

    var url = "http://tvapi-mashupjonne.rhcloud.com/api/channel/" + channel
    xhr.open("GET", url, true);

    xhr.onreadystatechange = function() {

        if(xhr.readyState === xhr.DONE) {
            if(xhr.status === 200) {
                var programs = JSON.parse(xhr.responseText);
                loaded(programs);
            }
        }
    }
    xhr.send();
}

function loaded(programs) {

//    var date1 = new Date();
//    var diff = 7200;
//    var currentIndex = 0;

    for( var index in programs ) {
        listModel.append({
                             "name": programs[index].data.name,
                             "start": programs[index].data.start,
                             "end": programs[index].data.end,
                             "description": programs[index].data.description
                         });

//        var date2 = new Date();
//        var time = programs[index].data.start.substr(11,5).split(":");
//        date2.setHours(time[0]);
//        date2.setMinutes(time[1]);

//        if(date2 >= date1) {

//            var temp = date2.getTime() - date1.getTime();

//            if(temp < diff) {
//                diff = temp;
//                currentIndex = index;
//            }

//        }

    }

//    console.log(new Date().toString());
//    console.log(programs[4].data.start.substr(11,5));

//    var dat = new Date();
//    time = programs[4].data.start.substr(11,5).split(":");
//    dat.setHours(time[0]);
//    dat.setMinutes(time[1]);

//    console.log(dat);

    // TODO calculate the current index of the ongoing program
    // listview.positionViewAtIndex(currentIndex,listview.Beginning);
    // listview.currentIndex = 5;
}
