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
    bool rollback();

    bool initializeInfoTable();
    bool createInfoTable();
    bool updateInfoTable(double version);

    bool createSeriesTable();
    bool createEpisodeTable();
    bool createBannerTable();

    bool deleteDuplicateEpisodes();
    bool deleteAllSeries();

    QVariantMap getNextEpisodeDetails(const int &seriesId) const;
    QString getStatus(const int &seriesId) const;
    QString getSeasonBanner(const int &seriesId, const int &season) const;
    QStringList getSeriesIds(bool includeEndedSeries);

    void markSeasonWatched(const int &seriesId, const int &seasonNumber);

    bool isAlreadyAdded(const int &seriesId, const QString &name);
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

    QMultiMap<int, QMap<QString, QStringList> > getMostWatchedDirectors();
    QMultiMap<int, QMap<QString, QStringList> > getMostWatchedActors();

signals:
    void populateTodayModel(QList<QVariantMap> allSeries);
    void populateBannerList(QList<QVariantMap> allSeries);
    void seriesDeleted();
    void seriesStored();
    void updateStatistics(QVariantMap);
    void populateSeasonList(QList<QVariantMap> seasons);
    void populateEpisodeList(QList<QVariantMap> episodes);

public slots:
    void getStartPageSeries();
    void getSeries();
    void getSeasons(int seriesId);
    void getEpisodes(int seriesId, int seasonNumber);
    void deleteSeries(const int &seriesId);
    void storeSeries(const QVariantMap &series);
    void storeEpisodes(const QString &seriesId, const QList<QVariantMap> &episodes);
    void getStatistics();
    void toggleWatched(QString episodeId, QString seriesId, int seasonNumber);
    void storePosterImageFor(QString seriesId, QString posterImage);
    void storeBannerImageFor(QString seriesId, QString bannerImage);
    void storeFanartImageFor(QString seriesId, QString fanartImage);
    void storeActors(const QString &seriesId, const QList<QVariantMap> &actors);
    void storeSeasonImages(const QString &seriesId, const QList<QVariantMap> &seasonImages);

private:
    QSqlDatabase m_db;
};

#endif // DATABASEMANAGER_H
