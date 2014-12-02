#include "episodedata.h"

EpisodeData::EpisodeData(QObject *parent) :
    QObject(parent)
{
}

EpisodeData::EpisodeData(QObject* parent, int ID, QString director, int epimgflag, QString episodeName,
                                     int episodeNumber, QString firstAired, QString guestStars, int IMDB_ID,
                                     QString language, QString overview, int productionCode, double rating,
                                     int ratingCount, int SeasonNumber, QString writer, int absoluteNumber,
                                     int airsAfterSeason, int airsBeforeSeason, QString filename, QString lastUpdated,
                                     int seasonID, int seriesID, QString thumbAdded, int thumbHeight, int thumbWidth,
                                     int watched):
    QObject(parent)
{
    myID = ID;
    myDirector = director;
    myEpimgflag = epimgflag;
    myEpisodeName = episodeName;
    myEpisodeNumber = episodeNumber;
    myFirstAired = firstAired;
    myGuestStars = guestStars;
    myIMDB_ID = IMDB_ID;
    myLanguage = language;
    myOverview = overview;
    myProductionCode = productionCode;
    myRating = rating;
    myRatingCount = ratingCount;
    mySeasonNumber = SeasonNumber;
    myWriter = writer;
    myAbsoluteNumber = absoluteNumber;
    myAirsAfterSeason = airsAfterSeason;
    myAirsBeforeSeason = airsBeforeSeason;
    myFilename = filename;
    myLastUpdated = lastUpdated;
    mySeasonID = seasonID;
    mySeriesID = seriesID;
    myThumbAdded = thumbAdded;
    myThumbHeight  = thumbHeight;
    myThumbWidth = thumbWidth;
    myWatched = watched;

}

int EpisodeData::getID() { return myID; }

QString EpisodeData::getDirector() { return myDirector; }

int EpisodeData::getEpimgflag() { return myEpimgflag; }

QString EpisodeData::getEpisodeName() { return myEpisodeName; }

int EpisodeData::getEpisodeNumber() { return myEpisodeNumber; }

QString EpisodeData::getFirstAired() { return myFirstAired; }

QString EpisodeData::getGuestStars() { return myGuestStars; }

int EpisodeData::getIMDB_ID() { return myIMDB_ID; }

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
