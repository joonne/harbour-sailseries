#ifndef STATISTICS_H
#define STATISTICS_H

#include <QObject>

#include "databasemanager.h"

class Statistics : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int WatchedEpisodesDuration READ getWatchedEpisodesDuration CONSTANT)
    Q_PROPERTY(int WatchedEpisodesCount READ getWatchedEpisodesCount CONSTANT)
    Q_PROPERTY(int AllEpisodesCount READ getAllEpisodesCount CONSTANT)
    Q_PROPERTY(int AllSeriesCount READ getAllSeriesCount CONSTANT)
    Q_PROPERTY(int AllSeasonsCount READ getAllSeasonsCount CONSTANT)
    Q_PROPERTY(int WatchedSeasonsCount READ getWatchedSeasonsCount CONSTANT)
    Q_PROPERTY(double AverageWatchedEpisodesDuration READ getAverageWatchedEpisodesDuration)
public:
    explicit Statistics(QObject *parent = 0, DatabaseManager *dbmanager = 0);

    Q_INVOKABLE void updateStatistics();

    int getWatchedEpisodesDuration();
    int getWatchedEpisodesCount();
    int getAllEpisodesCount();
    int getAllSeriesCount();
    int getAllSeasonsCount();
    int getWatchedSeasonsCount();
    double getAverageWatchedEpisodesDuration();

signals:

public slots:

private:
    DatabaseManager* m_dbmanager;

    int m_watchedEpisodesDuration;
    int m_watchedEpisodesCount;
    int m_allEpisodesCount;
    int m_allSeriesCount;
    int m_allSeasonsCount;
    int m_watchedSeasonsCount;
    double m_averageWatchedEpisodesDuration;
};

#endif // STATISTICS_H
