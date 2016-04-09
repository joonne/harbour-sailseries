#include "episodedata.h"

EpisodeData::EpisodeData(QObject *parent, QMap<QString, QString> params) :
    QObject(parent)
{
    myID = params["id"].toInt();
    myDirector = params["director"];
    myEpimgflag = 0;
    myEpisodeName = params["episodeName"];
    myEpisodeNumber = params["episodeNumber"].toInt();
    myFirstAired = params["firstAired"];
    myGuestStars = params["guestStars"];
    myIMDB_ID = "";
    myLanguage = "";
    myOverview = params["overview"];
    myProductionCode = 0;
    myRating = 0;
    myRatingCount = 0;
    mySeasonNumber = params["seasonNumber"].toInt();
    myWriter = params["writer"];
    myAbsoluteNumber = params["absoluteNumber"].toInt();
    myAirsAfterSeason = 0;
    myAirsBeforeSeason = 0;
    myFilename = params["filename"];
    myLastUpdated = "";
    mySeasonID = 0;
    mySeriesID = 0;
    myThumbAdded = "";
    myThumbHeight  = 0;
    myThumbWidth = 0;
    myWatched = params["watched"].toInt();
}

int EpisodeData::getID() { return myID; }

QString EpisodeData::getDirector() { return myDirector; }

int EpisodeData::getEpimgflag() { return myEpimgflag; }

QString EpisodeData::getEpisodeName() { return myEpisodeName; }

int EpisodeData::getEpisodeNumber() { return myEpisodeNumber; }

QString EpisodeData::getFirstAired() { return myFirstAired; }

QString EpisodeData::getGuestStars() { return myGuestStars; }

QString EpisodeData::getIMDB_ID() { return myIMDB_ID; }

QString EpisodeData::getLanguage() { return myLanguage; }

QString EpisodeData::getOverview() { return myOverview; }

int EpisodeData::getProductionCode() { return myProductionCode; }

double EpisodeData::getRating() { return myRating; }

int EpisodeData::getRatingCount() { return myRatingCount; }

int EpisodeData::getSeasonNumber() { return mySeasonNumber; }

QString EpisodeData::getWriter() { return myWriter; }

int EpisodeData::getAbsoluteNumber() { return myAbsoluteNumber; }

int EpisodeData::getAirsAfterSeason() { return myAirsAfterSeason; }

int EpisodeData::getAirsBeforeSeason() { return myAirsBeforeSeason; }

QString EpisodeData::getFilename() { return myFilename; }

QString EpisodeData::getLastUpdated() { return myLastUpdated; }

int EpisodeData::getSeasonID() { return mySeasonID; }

int EpisodeData::getSeriesID() { return mySeriesID; }

QString EpisodeData::getThumbAdded() { return myThumbAdded; }

int EpisodeData::getThumbHeight() { return myThumbHeight; }

int EpisodeData::getThumbWidth() { return myThumbWidth; }

int EpisodeData::getWatched() { return myWatched; }

void EpisodeData::setWatched(int watched) {
    if(myWatched != watched) {
        myWatched = watched;
        emit watchedChanged();
    }
}

QString EpisodeData::getWatchedFlagImage() {
    return myWatchedFlagImage;
}

void EpisodeData::setWatchedFlagImage(QString imagepath) {
    if(myWatchedFlagImage != imagepath) {
        myWatchedFlagImage = imagepath;
        emit watchedFlagImageChanged();
    }
}
