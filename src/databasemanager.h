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

    bool openDB();
    bool deleteDB();
    QSqlError lastError();
    void close();
    bool createDB();
    void setUpDB();

    bool startTransaction();
    bool commit();

    bool initializeInfoTable();
    bool createInfoTable();
    bool updateInfoTable(double version);

    bool createSeriesTable();
    bool createEpisodeTable();
    bool createBannerTable();

    bool deleteDuplicateEpisodes();

    bool insertSeries(QVariantMap series);
    bool insertEpisodes(QList<QVariantMap> episodes, int seriesId);
    bool insertBanners(QList<QVariantMap> banners, int seriesId);

    bool updateSeries();
    bool updateEpisode();

    bool deleteAllSeries();

    QVariantMap getNextEpisodeDetails(int seriesID);
    QString getStatus(int seriesID);
    QString getSeasonBanner(int seriesID, int season);
    QStringList getSeriesIds(bool updateEndedSeries);

    void markSeasonWatched(int seriesID, int season);

    bool isAlreadyAdded(int seriesId, QString name);
    int watchedCount(int seriesID);
    int watchedCountBySeason(int seriesID, int seasonNumber);
    int totalCount(int seriesID);
    int totalCountBySeason(int seriesID, int seasonNumber);
    int seasonCount(int seriesID);

    int getWatchedEpisodesDuration();
    double getAverageWatchedEpisodesDuration();
    int getWatchedEpisodesCount();
    int getAllEpisodesCount();
    int getAllSeriesCount();
    int getEndedSeriesCount();
    int getWatchedSeriesCount();
    int getWatchedSeasonsCount();
    int getAllSeasonsCount();

    QMultiMap<int, QMap<QString, QStringList> > getMostWatchedDirectors();
    QMultiMap<int, QMap<QString, QStringList> > getMostWatchedActors();

signals:
    void populateTodayModel(QList<QVariantMap> allSeries);
    void populateBannerList(QList<QVariantMap> allSeries);
    void seriesDeleted(bool success);
    void seriesStored();
    void updateStatistics(QVariantMap);
    void populateSeasonList(QList<QVariantMap> seasons);
    void populateEpisodeList(QList<QVariantMap> episodes);

public slots:
    void getStartPageSeries();
    void getSeries();
    void getSeasons(int seriesId);
    void getEpisodes(int seriesId, int seasonNumber);
    void deleteSeries(int seriesId);
    void storeSeries(QList<QVariantMap> series, QList<QVariantMap> episodes, QList<QVariantMap> banners);
    void getStatistics();
    void toggleWatched(QString episodeId, QString seriesId, int seasonNumber);

private:
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
