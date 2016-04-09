#include "seriesdata.h"

SeriesData::SeriesData(QObject *parent, QMap<QString, QString> params) :
    QObject(parent)
{
    myID = params["id"];
    myActors = params["actors"];
    myAirsDayOfWeek = params["airsDayOfWeek"];
    myAirsTime = params["airsTime"];
    myContentRating = params["contentRating"];
    myFirstAired = params["firstAired"];
    myGenre = params["genre"];
    myIMDB_ID = params["imdbId"];
    myLanguage = params["language"];
    myNetwork = params["network"];
    myOverview = params["overview"];
    myRating = params["rating"];
    myRatingCount = params["ratingCount"].toInt();
    myRuntime = params["runtime"].toInt();
    mySeriesName = params["seriesName"];
    myAliasNames = params["aliasNames"];
    myStatus = params["status"];
    myAdded = params["added"];
    myAddedBy = params["addedBy"].toInt();
    myBanner = params["banner"];
    myFanart = params["fanart"];
    myLastUpdated = params["lastUpdated"];
    myPoster = params["poster"];
    myZap2it_ID = params["zap2itid"];
    myWatched = params["watched"].toInt();
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
