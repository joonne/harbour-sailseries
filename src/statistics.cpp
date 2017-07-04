#include "statistics.h"

Statistics::Statistics(QObject *parent, DatabaseManager *dbmanager) :
    QObject(parent), m_dbmanager(dbmanager)
{
    connect(this,
            SIGNAL(statisticsRequested()),
            m_dbmanager,
            SLOT(getStatistics()));

    connect(m_dbmanager,
            SIGNAL(updateStatistics(QVariantMap)),
            this,
            SLOT(updateStatistics(QVariantMap)));

    emit statisticsRequested();
}

void Statistics::requestStatistics()
{
    emit statisticsRequested();
}

void Statistics::updateStatistics(QVariantMap statistics)
{
    m_watchedEpisodesDuration = statistics["watchedEpisodesDuration"].toInt();
    emit watchedEpisodesDurationChanged();

    m_watchedEpisodesCount = statistics["watchedEpisodesCount"].toInt();
    emit watchedEpisodesCountChanged();

    m_allEpisodesCount = statistics["allEpisodesCount"].toInt();
    emit allEpisodesCountChanged();

    m_allSeriesCount = statistics["allSeriesCount"].toInt();
    emit allSeriesCountChanged();

    m_watchedSeriesCount = statistics["watchedSeriesCount"].toInt();
    emit watchedSeriesCountChanged();

    m_allSeasonsCount = statistics["allSeasonsCount"].toInt();
    emit allSeasonsCountChanged();

    m_watchedSeasonsCount = statistics["watchedSeasonsCount"].toInt();
    emit watchedSeasonsCountChanged();

    m_averageWatchedEpisodesDuration = statistics["averageWatchedEpisodesDuration"].toDouble();
    emit averageWatchedEpisodesDurationChanged();
}

int Statistics::getWatchedEpisodesDuration() { return m_watchedEpisodesDuration; }

int Statistics::getWatchedEpisodesCount() { return m_watchedEpisodesCount; }

int Statistics::getAllEpisodesCount() { return m_allEpisodesCount; }

int Statistics::getAllSeriesCount() { return m_allSeriesCount; }

int Statistics::getWatchedSeriesCount() { return m_watchedSeriesCount; }

int Statistics::getAllSeasonsCount() { return m_allSeasonsCount; }

int Statistics::getWatchedSeasonsCount() { return m_watchedSeasonsCount; }

double Statistics::getAverageWatchedEpisodesDuration() { return m_averageWatchedEpisodesDuration; }
