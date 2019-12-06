#ifndef STATISTICS_H
#define STATISTICS_H

#include <QObject>

#include "databasemanager.h"

class Statistics : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int WatchedEpisodesDuration READ getWatchedEpisodesDuration NOTIFY watchedEpisodesDurationChanged)
    Q_PROPERTY(int WatchedEpisodesCount READ getWatchedEpisodesCount NOTIFY watchedEpisodesCountChanged)
    Q_PROPERTY(int AllEpisodesCount READ getAllEpisodesCount NOTIFY allEpisodesCountChanged)
    Q_PROPERTY(int AllSeriesCount READ getAllSeriesCount NOTIFY allSeriesCountChanged)
    Q_PROPERTY(int WatchedSeriesCount READ getWatchedSeriesCount NOTIFY watchedSeriesCountChanged)
    Q_PROPERTY(int AllSeasonsCount READ getAllSeasonsCount NOTIFY allSeasonsCountChanged)
    Q_PROPERTY(int WatchedSeasonsCount READ getWatchedSeasonsCount NOTIFY watchedSeasonsCountChanged)
    Q_PROPERTY(double AverageWatchedEpisodesDuration READ getAverageWatchedEpisodesDuration NOTIFY averageWatchedEpisodesDurationChanged)

public:
    explicit Statistics(QObject *parent = 0, DatabaseManager *dbmanager = 0);

    Q_INVOKABLE void requestStatistics();

    int getWatchedEpisodesDuration();
    int getWatchedEpisodesCount();
    int getAllEpisodesCount();
    int getAllSeriesCount();
    int getWatchedSeriesCount();
    int getAllSeasonsCount();
    int getWatchedSeasonsCount();
    double getAverageWatchedEpisodesDuration();

signals:
    void watchedEpisodesDurationChanged();
    void watchedEpisodesCountChanged();
    void allEpisodesCountChanged();
    void allSeriesCountChanged();
    void watchedSeriesCountChanged();
    void allSeasonsCountChanged();
    void watchedSeasonsCountChanged();
    void averageWatchedEpisodesDurationChanged();

    void statisticsRequested();

public slots:
    void updateStatistics(QVariantMap statistics);

private:
    DatabaseManager* m_dbmanager;
    int m_watchedEpisodesDuration;
    int m_watchedEpisodesCount;
    int m_allEpisodesCount;
    int m_allSeriesCount;
    int m_watchedSeriesCount;
    int m_allSeasonsCount;
    int m_watchedSeasonsCount;
    double m_averageWatchedEpisodesDuration;
};

#endif // STATISTICS_H
