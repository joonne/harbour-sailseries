#include "statistics.h"

Statistics::Statistics(QObject *parent, DatabaseManager *dbmanager) : QObject(parent), m_dbmanager(dbmanager) {

    updateStatistics();

}

void Statistics::updateStatistics() {

    m_watchedEpisodesDuration = m_dbmanager->getWatchedEpisodesDuration();
    emit watchedEpisodesDurationChanged();

    m_watchedEpisodesCount = m_dbmanager->getWatchedEpisodesCount();
    emit watchedEpisodesCountChanged();

    m_allEpisodesCount = m_dbmanager->getAllEpisodesCount();
    emit allEpisodesCountChanged();

    m_allSeriesCount = m_dbmanager->getAllSeriesCount();
    emit allSeriesCountChanged();

    m_watchedSeriesCount = m_dbmanager->getWatchedSeriesCount();
    emit watchedSeriesCountChanged();

    m_allSeasonsCount = m_dbmanager->getAllSeasonsCount();
    emit allSeasonsCountChanged();

    m_watchedSeasonsCount = m_dbmanager->getWatchedSeasonsCount();
    emit watchedSeasonsCountChanged();

    m_averageWatchedEpisodesDuration = m_dbmanager->getAverageWatchedEpisodesDuration();
    emit averageWatchedEpisodesDurationChanged();

    qDebug() << m_dbmanager->getMostWatchedActors();
//    auto actor = mostWacthedActors.end().value().firstKey();
//    auto actedSeries = mostWacthedActors.end().value().first().join(",");
//    m_mostWatchedActor = new Actor(actor, actedSeries);

//    auto mostWatchedDirectors = m_dbmanager->getMostWatchedDirectors();
//    qDebug() << mostWatchedDirectors;

//    auto director = mostWatchedDirectors.end().value();
//    qDebug() << "director" << director;

//    auto directedSeries = mostWatchedDirectors.end().value();
//    qDebug() << "directedSeries" << directedSeries;
//    directedSeries.removeDuplicates();
//    m_mostWacthedDirector = new Director(this, director, directedSeries.join(","));
}

int Statistics::getWatchedEpisodesDuration() { return m_watchedEpisodesDuration; }

int Statistics::getWatchedEpisodesCount() { return m_watchedEpisodesCount; }

int Statistics::getAllEpisodesCount() { return m_allEpisodesCount; }

int Statistics::getAllSeriesCount() { return m_allSeriesCount; }

int Statistics::getWatchedSeriesCount() { return m_watchedSeriesCount; }

int Statistics::getAllSeasonsCount() { return m_allSeasonsCount; }

int Statistics::getWatchedSeasonsCount() { return m_watchedSeasonsCount; }

double Statistics::getAverageWatchedEpisodesDuration() { return m_averageWatchedEpisodesDuration; }
