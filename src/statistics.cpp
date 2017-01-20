#include "statistics.h"

Statistics::Statistics(QObject *parent, DatabaseManager *dbmanager) : QObject(parent), m_dbmanager(dbmanager) {

    updateStatistics();

}

void Statistics::updateStatistics() {

    m_watchedEpisodesDuration = m_dbmanager->getWatchedEpisodesDuration();
    m_watchedEpisodesCount = m_dbmanager->getWatchedEpisodesCount();
    m_allEpisodesCount = m_dbmanager->getAllEpisodesCount();
    m_allSeriesCount = m_dbmanager->getAllSeriesCount();
    m_allSeasonsCount = m_dbmanager->getAllSeasonsCount();
    m_watchedSeasonsCount = m_dbmanager->getWatchedSeasonsCount();
    m_averageWatchedEpisodesDuration = m_dbmanager->getAverageWatchedEpisodesDuration();
}

int Statistics::getWatchedEpisodesDuration() { return m_watchedEpisodesDuration; }

int Statistics::getWatchedEpisodesCount() { return m_watchedEpisodesCount; }

int Statistics::getAllEpisodesCount() { return m_allEpisodesCount; }

int Statistics::getAllSeriesCount() { return m_allSeriesCount; }

int Statistics::getAllSeasonsCount() { return m_allSeasonsCount; }

int Statistics::getWatchedSeasonsCount() { return m_watchedSeasonsCount; }

double Statistics::getAverageWatchedEpisodesDuration() { return m_averageWatchedEpisodesDuration; }
