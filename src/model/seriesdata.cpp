#include "seriesdata.h"

SeriesData::SeriesData(QObject *parent, QVariantMap params) :
    QObject(parent)
{

    m_id = params["id"].toInt();
    m_actors = params["actors"].toString();
    m_airsDayOfWeek = params["airsDayOfWeek"].toString();
    m_airsTime = params["airsTime"].toString();
    m_contentRating = params["contentRating"].toString();
    m_firstAired = params["firstAired"].toString();
    m_genre = params["genre"].toString();
    m_imdbId = params.contains("imdbId") ? params["imdbId"].toString() : params["imdbID"].toString();
    m_language = params["language"].toString();
    m_network = params["network"].toString();
    m_overview = params["overview"].toString();
    m_rating = params["rating"].toString();
    m_ratingCount = params["ratingCount"].toInt();
    m_runtime = params["runtime"].toInt();
    m_seriesName = params["seriesName"].toString();
    m_aliasNames = params["aliasNames"].toString();
    m_status = params["status"].toString();
    m_added = params["added"].toString();
    m_addedBy = params["addedBy"].toInt();
    m_banner = params["banner"].toString();
    m_fanart = params["fanart"].toString();
    m_lastUpdated = params["lastUpdated"].toString();
    m_poster = params["poster"].toString();
    m_zap2itId = params["zap2itid"].toString();
    m_watched = params["watched"].toInt();
    m_isAdded = params["isAdded"].toBool();
    m_nextEpisodeId = params["nextEpisodeId"].toString();
    m_nextEpisodeName = params["nextEpisodeName"].toString();
    m_nextEpisodeNumber = params["nextEpisodeNumber"].toString();
    m_nextEpisodeSeasonNumber = params["nextEpisodeSeasonNumber"].toString();
    m_daysToNextEpisode = params["daysToNext"].toString();
    m_nextEpisodeFirstAired = params["nextEpisodeFirstAired"].toString();
    m_nextEpisodeBanner = params["nextEpisodeBanner"].toString();
    m_nextEpisodeOverview = params["nextEpisodeOverview"].toString();
    m_nextEpisodeGuestStars = params["nextEpisodeGuestStars"].toString();
    m_nextEpisodeWriter = params["nextEpisodeWriter"].toString();
    m_nextEpisodeWatched = params["nextEpisodeWatched"].toInt();
}

int SeriesData::getID() { return m_id; }

QString SeriesData::getActors() { return m_actors; }

QString SeriesData::getAirsDayOfWeek() { return m_airsDayOfWeek; }

QString SeriesData::getAirsTime() { return m_airsTime; }

QString SeriesData::getContentRating() { return m_contentRating; }

QString SeriesData::getGenre() { return m_genre; }

QString SeriesData::getRating() { return m_rating; }

int SeriesData::getRatingCount() { return m_ratingCount; }

int SeriesData::getRuntime() { return m_runtime; }

QString SeriesData::getStatus() { return m_status; }

QString SeriesData::getAdded() { return m_added; }

int SeriesData::getAddedBy() { return m_addedBy; }

QString SeriesData::getFanart() { return m_fanart; }

QString SeriesData::getLastUpdated() { return m_lastUpdated; }

QString SeriesData::getPoster() { return m_poster; }

int SeriesData::getWatched() { return m_watched; }

QString SeriesData::getLanguage() { return m_language; }

QString SeriesData::getSeriesName() { return m_seriesName; }

QString SeriesData::getAliasNames() { return m_aliasNames; }

QString SeriesData::getBanner() { return m_banner; }

QString SeriesData::getOverview() { return m_overview; }

QString SeriesData::getFirstAired() { return m_firstAired; }

QString SeriesData::getIMDB_ID() { return m_imdbId; }

QString SeriesData::getZap2it_ID() { return m_zap2itId; }

QString SeriesData::getNetwork() { return m_network; }

void SeriesData::setWatched(int watched)
{
    if (m_watched != watched) {
        m_watched = watched;
        emit watchedChanged();
    }
}

bool SeriesData::getIsAdded() { return m_isAdded; }

QString SeriesData::getNextEpisodeId() { return m_nextEpisodeId; }

QString SeriesData::getNextEpisodeName() { return m_nextEpisodeName; }

QString SeriesData::getNextEpisodeNumber() { return m_nextEpisodeNumber; }

QString SeriesData::getNextEpisodeSeasonNumber() { return m_nextEpisodeSeasonNumber; }

QString SeriesData::getDaysToNextEpisode() { return m_daysToNextEpisode; }

QString SeriesData::getWatchedCount() { return m_watchedCount; }

QString SeriesData::getTotalCount() { return m_totalCount; }

QString SeriesData::getNextEpisodeFirstAired() { return m_nextEpisodeFirstAired; }

QString SeriesData::getNextEpisodeBanner() { return m_nextEpisodeBanner; }

QString SeriesData::getNextEpisodeOverview() { return m_nextEpisodeOverview; }

QString SeriesData::getNextEpisodeGuestStars() { return m_nextEpisodeGuestStars; }

QString SeriesData::getNextEpisodeWriter() { return m_nextEpisodeWriter; }

int SeriesData::getNextEpisodeWatched() { return m_nextEpisodeWatched; }

void SeriesData::setNextEpisodeWatched(int watched)
{
    if (m_nextEpisodeWatched != watched) {
        m_nextEpisodeWatched = watched;
        emit nextEpisodeWatchedChanged();
    }
}
