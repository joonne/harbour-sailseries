#include "seriesdata.h"

SeriesData::SeriesData(QObject *parent, QMap<QString, QString> params) :
    QObject(parent)
{

    m_id = params["id"];
    m_actors = params["actors"];
    m_airsDayOfWeek = params["airsDayOfWeek"];
    m_airsTime = params["airsTime"];
    m_contentRating = params["contentRating"];
    m_firstAired = params["firstAired"];
    m_genre = params["genre"];
    m_imdbId = params.contains("imdbId") ? params["imdbId"] : params["IMDB_ID"];
    m_language = params["language"];
    m_network = params.contains("network") ? params["network"] : params["Network"];
    m_overview = params.contains("overview") ? params["overview"] : params["Overview"];
    m_rating = params["rating"];
    m_ratingCount = params["ratingCount"].toInt();
    m_runtime = params["runtime"].toInt();
    m_seriesName = params.contains("seriesName") ? params["seriesName"] : params["SeriesName"];
    m_aliasNames = params["aliasNames"];
    m_status = params["status"];
    m_added = params["added"];
    m_addedBy = params["addedBy"].toInt();
    m_banner = params["banner"];
    m_fanart = params["fanart"];
    m_lastUpdated = params["lastUpdated"];
    m_poster = params["poster"];
    m_zap2itId = params["zap2itid"];
    m_watched = params["watched"].toInt();
    m_nextEpisodeId = params["nextEpisodeId"];
    m_nextEpisodeName = params["nextEpisodeName"];
    m_nextEpisodeNumber = params["nextEpisodeNumber"];
    m_nextEpisodeSeasonNumber = params["nextEpisodeSeasonNumber"];
    m_daysToNextEpisode = params["daysToNext"];
    m_nextEpisodeFirstAired = params["nextEpisodeFirstAired"];
    m_nextEpisodeBanner = params["nextEpisodeBanner"];
    m_nextEpisodeOverview = params["nextEpisodeOverview"];
    m_nextEpisodeGuestStars = params["nextEpisodeGuestStars"];
    m_nextEpisodeWriter = params["nextEpisodeWriter"];
    m_nextEpisodeWatched = params["nextEpisodeWatched"].toInt();
}

QString SeriesData::getID() { return m_id; }

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
