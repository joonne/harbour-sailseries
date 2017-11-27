.import "http.js" as HTTP
.import "promise.js" as Promise

var apiUrl = 'http://tv-api-tv-api.1d35.starter-us-east-1.openshiftapps.com/api';

function getPrograms(channel) {
    var url = apiUrl + '/channels/' + channel + '/programs';
    return HTTP.get(url);
}

function getChannels(country) {
    var url = apiUrl + '/channels' + '?country=' + country;
    return HTTP.get(url);
}

function getCountries() {
    var url = apiUrl + '/countries';
    return HTTP.get(url);
}
