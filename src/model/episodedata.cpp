#include "episodedata.h"

EpisodeData::EpisodeData(QObject *parent, QVariantMap params) :
    QObject(parent)
{
    m_id = params["id"].toInt();
    m_director = params["director"].toString();
    m_epimgflag = 0;
    m_episodeName = params["episodeName"].toString();
    m_episodeNumber = params["episodeNumber"].toInt();
    m_firstAired = params["firstAired"].toString();
    m_guestStars = params["guestStars"].toString();
    m_imdbId = "";
    m_language = "";
    m_overview = params["overview"].toString();
    m_productionCode = 0;
    m_rating = 0;
    m_ratingCount = 0;
    m_seasonNumber = params["seasonNumber"].toInt();
    m_writer = params["writer"].toString();
    m_absoluteNumber = params["absoluteNumber"].toInt();
    m_airsAfterSeason = 0;
    m_airsBeforeSeason = 0;
    m_filename = params["filename"].toString();
    m_lastUpdated = "";
    m_seasonId = 0;
    m_seriesId = 0;
    m_thumbAdded = "";
    m_thumbHeight  = 0;
    m_thumbWidth = 0;
    m_watched = params["watched"].toInt();
}

int EpisodeData::getID() { return m_id; }

QString EpisodeData::getDirector() { return m_director; }

int EpisodeData::getEpimgflag() { return m_epimgflag; }

QString EpisodeData::getEpisodeName() { return m_episodeName; }

int EpisodeData::getEpisodeNumber() { return m_episodeNumber; }

QString EpisodeData::getFirstAired() { return m_firstAired; }

QString EpisodeData::getGuestStars() { return m_guestStars; }

QString EpisodeData::getIMDB_ID() { return m_imdbId; }

QString EpisodeData::getLanguage() { return m_language; }

QString EpisodeData::getOverview() { return m_overview; }

int EpisodeData::getProductionCode() { return m_productionCode; }

double EpisodeData::getRating() { return m_rating; }

int EpisodeData::getRatingCount() { return m_ratingCount; }

int EpisodeData::getSeasonNumber() { return m_seasonNumber; }

QString EpisodeData::getWriter() { return m_writer; }

int EpisodeData::getAbsoluteNumber() { return m_absoluteNumber; }

int EpisodeData::getAirsAfterSeason() { return m_airsAfterSeason; }

int EpisodeData::getAirsBeforeSeason() { return m_airsBeforeSeason; }

QString EpisodeData::getFilename() { return m_filename; }

QString EpisodeData::getLastUpdated() { return m_lastUpdated; }

int EpisodeData::getSeasonID() { return m_seasonId; }

int EpisodeData::getSeriesID() { return m_seriesId; }

QString EpisodeData::getThumbAdded() { return m_thumbAdded; }

int EpisodeData::getThumbHeight() { return m_thumbHeight; }

int EpisodeData::getThumbWidth() { return m_thumbWidth; }

int EpisodeData::getWatched() { return m_watched; }

void EpisodeData::setWatched(int watched)
{
    if (m_watched != watched) {
        m_watched = watched;
        emit watchedChanged();
    }
}
