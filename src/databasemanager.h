#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QFile>
#include <QDir>
#include <QtSql/QSqlQuery>
#include <QTextStream>
#include <QStandardPaths>
#include <QVariant>
#include <QDebug>
#include <QBuffer>
#include <QStringRef>
#include <QDate>
#include <QLocale>
#include <QDateTime>

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = 0);
    ~DatabaseManager();

    bool open();
    void close();
    void migrate();

    bool transaction();
    bool commit();
    bool rollback();

    bool createInfoTable();
    bool updateInfoTable(double version);

    bool createSeriesTable();
    bool createEpisodeTable();
    bool createBannerTable();
    bool addRuntimeToEpisode();

    QVariantMap getNextEpisodeDetails(const int &seriesId) const;
    QString getStatus(const int &seriesId) const;
    QString getSeasonBanner(const int &seriesId, const int &season) const;

    int watchedCount(int seriesId);
    int watchedCountBySeason(int seriesId, int seasonNumber);
    int totalCount(int seriesId);
    int totalCountBySeason(int seriesId, int seasonNumber);
    int seasonCount(int seriesId);

    int getWatchedEpisodesDuration();
    double getAverageWatchedEpisodesDuration();
    int getWatchedEpisodesCount();
    int getAllEpisodesCount();
    int getAllSeriesCount();
    int getEndedSeriesCount();
    int getWatchedSeriesCount();
    int getWatchedSeasonsCount();
    int getAllSeasonsCount();

signals:
    void populateTodayModel(QList<QVariantMap>);
    void populateBannerList(QList<QVariantMap>);
    void seriesDeleted();
    void seriesStored(int);
    void updateStatistics(QVariantMap);
    void populateSeasonList(QList<QVariantMap>);
    void populateEpisodeList(QList<QVariantMap>);
    void getSeriesNamesReady(QSet<QString>);
    void getSeriesIdsReady(QList<int>);
    void setWatchedReady(int, bool);

public slots:
    void getStartPageSeries();
    void getSeries();
    void getSeasons(const int &seriesId);
    void getEpisodes(const int &seriesId, const int &seasonNumber);
    void deleteSeries(const int &seriesId);
    void storeSeries(const QVariantMap &series);
    void storeEpisodes(const int &seriesId, const QList<QVariantMap> &episodes);
    void getStatistics();
    void setWatched(const int &episodeId, const int &seriesId, const bool &watched);
    void storeSeasonImages(const int &seriesId, const QList<QVariantMap> &seasonImages);
    void markSeasonAsWatched(const int &seriesId, const int &seasonNumber);
    void getSeriesIds(const bool &includeEndedSeries);
    void getSeriesNames();

private:
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
