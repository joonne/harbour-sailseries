#include "seriesdata.h"

SeriesData::SeriesData(QObject *parent, QMap<QString, QString> params) :
    QObject(parent)
{
    myID = params["id"] || "";
    myActors = params["actors"] || "";
    myAirsDayOfWeek = params["airsDayOfWeek"] || "";
    myAirsTime = params["airsTime"];
    myContentRating = params["contentRating"];
    myFirstAired = params["firstAired"];
    myGenre = params["genre"];
    myIMDB_ID = IMDB_ID;
    myLanguage = language;
    myNetwork = network;
    myOverview = overview;
    myRating = rating;
    myRatingCount = ratingCount;
    myRuntime = runtime;
    mySeriesName = seriesName;
    //myAliasNames = aliasNames;
    myStatus = status;
    myAdded = added;
    myAddedBy = addedby;
    myBanner = banner;
    myFanart = fanart;
    myLastUpdated = lastUpdated;
    myPoster = poster;
    myZap2it_ID = zap2itid;
    myWatched = watched;
}

SeriesData::SeriesData(QObject *parent, QString id, QString language, QString seriesName,
                       QString aliasNames, QString banner, QString overview, QString firstAired,
                       QString IMDB_ID, QString zap2it_ID, QString network):
    QObject(parent)
{
    myID = id;
    myLanguage = language;
    mySeriesName = seriesName;
    myAliasNames = aliasNames;
    myBanner = banner;
    myOverview = overview;
    myFirstAired = firstAired;
    myIMDB_ID = IMDB_ID;
    myZap2it_ID = zap2it_ID;
    myNetwork = network;

}

SeriesData::SeriesData(QObject* parent, QString id, QString actors, QString airsDayOfWeek,
                       QString airsTime, QString contentRating,
                       QString firstAired, QString genre, QString IMDB_ID,
                       QString language, QString network, QString overview,
                       double rating, int ratingCount, int runtime, QString seriesName,
                       QString status, QString added, int addedby, QString banner,
                       QString fanart, QString lastUpdated, QString poster, QString zap2itid, int watched):
    QObject(parent)
{
    myID = id;
    myActors = actors;
    myAirsDayOfWeek = airsDayOfWeek;
    myAirsTime = airsTime;
    myContentRating = contentRating;
    myFirstAired = firstAired;
    myGenre = genre;
    myIMDB_ID = IMDB_ID;
    myLanguage = language;
    myNetwork = network;
    myOverview = overview;
    myRating = rating;
    myRatingCount = ratingCount;
    myRuntime = runtime;
    mySeriesName = seriesName;
    //myAliasNames = aliasNames;
    myStatus = status;
    myAdded = added;
    myAddedBy = addedby;
    myBanner = banner;
    myFanart = fanart;
    myLastUpdated = lastUpdated;
    myPoster = poster;
    myZap2it_ID = zap2itid;
    myWatched = watched;


}

SeriesData::SeriesData(QObject* parent, QString banner, QString poster,
                       QString seriesName, QString status, QString id,
                       QString overview, QString imdbID, QString rating,
                       QString nextEpisodeName, QString nextEpisodeNumber,
                       QString nextEpisodeSeasonNumber, QString daysToNextEpisode,
                       QString watchedCount, QString totalCount):
    QObject(parent){

    myBanner = banner;
    myPoster = poster;
    mySeriesName = seriesName;
    myStatus = status;
    myID = id;
    myOverview = overview;
    myIMDB_ID = imdbID;
    myRating = rating;
    myNextEpisodeName = nextEpisodeName;
    myNextEpisodeNumber = nextEpisodeNumber;
    myNextEpisodeSeasonNumber = nextEpisodeSeasonNumber;
    myDaysToNextEpisode = daysToNextEpisode;
    myWatchedCount = watchedCount;
    myTotalCount = totalCount;
}

SeriesData::SeriesData(QObject *parent, QString seriesName, QString network, QString airsTime,
                       QString nextEpisodeName, QString nextEpisodeNumber, QString nextEpisodeSeasonNumber):
    QObject(parent){

    mySeriesName = seriesName;
    myNetwork = network;
    myAirsTime = airsTime;
    myNextEpisodeName = nextEpisodeName;
    myNextEpisodeNumber = nextEpisodeNumber;
    myNextEpisodeSeasonNumber = nextEpisodeSeasonNumber;

}

QString SeriesData::getID() { return myID; }

QString SeriesData::getActors() { return myActors; }

QString SeriesData::getAirsDayOfWeek() { return myAirsDayOfWeek; }

QString SeriesData::getAirsTime() { return myAirsTime; }

QString SeriesData::getContentRating() { return myContentRating; }

QString SeriesData::getGenre() { return myGenre; }

QString SeriesData::getRating() { return myRating; }

int SeriesData::getRatingCount() { return myRatingCount; }

int SeriesData::getRuntime() { return myRuntime; }

QString SeriesData::getStatus() { return myStatus; }

QString SeriesData::getAdded() { return myAdded; }

int SeriesData::getAddedBy() { return myAddedBy; }

QString SeriesData::getFanart() { return myFanart; }

QString SeriesData::getLastUpdated() { return myLastUpdated; }

QString SeriesData::getPoster() { return myPoster; }

int SeriesData::getWatched() { return myWatched; }

QString SeriesData::getLanguage() { return myLanguage; }

QString SeriesData::getSeriesName() { return mySeriesName; }

QString SeriesData::getAliasNames() { return myAliasNames; }

QString SeriesData::getBanner() { return myBanner; }

QString SeriesData::getOverview() { return myOverview; }

QString SeriesData::getFirstAired() { return myFirstAired; }

QString SeriesData::getIMDB_ID() { return myIMDB_ID; }

QString SeriesData::getZap2it_ID() { return myZap2it_ID; }

QString SeriesData::getNetwork() { return myNetwork; }

void SeriesData::setWatched(int watched) {

    if(myWatched != watched) {
        myWatched = watched;
        emit watchedChanged();
    }
}

QString SeriesData::getNextEpisodeName() { return myNextEpisodeName; }

QString SeriesData::getNextEpisodeNumber() { return myNextEpisodeNumber; }

QString SeriesData::getNextEpisodeSeasonNumber() { return myNextEpisodeSeasonNumber; }

QString SeriesData::getDaysToNextEpisode() { return myDaysToNextEpisode; }

QString SeriesData::getWatchedCount() { return myWatchedCount; }

QString SeriesData::getTotalCount() { return myTotalCount; }
