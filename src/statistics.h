#ifndef STATISTICS_H
#define STATISTICS_H

#include <QObject>

#include "databasemanager.h"

class Statistics : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int WatchedEpisodesDuration READ getWatchedEpisodesDuration)
    Q_PROPERTY(int WatchedEpisodesCount READ getWatchedEpisodesCount)
    Q_PROPERTY(int AllEpisodesCount READ getAllEpisodesCount)
    Q_PROPERTY(int AllSeriesCount READ getAllSeriesCount)
    Q_PROPERTY(int AllSeasonsCount READ getAllSeasonsCount)
public:
    explicit Statistics(QObject *parent = 0, DatabaseManager *dbmanager = 0);
    void updateStatistics();

    int getWatchedEpisodesDuration();
    int getWatchedEpisodesCount();
    int getAllEpisodesCount();
    int getAllSeriesCount();
    int getAllSeasonsCount();

signals:

public slots:

private:
    DatabaseManager* m_dbmanager;

    int m_watchedEpisodesDuration;
    int m_watchedEpisodesCount;
    int m_allEpisodesCount;
    int m_allSeriesCount;
    int m_allSeasonsCount;
};

#endif // STATISTICS_H
