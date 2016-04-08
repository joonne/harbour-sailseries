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
#include <QPixmap>
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

    void setUpDB();

    bool openDB();
    bool deleteDB();
    QSqlError lastError();
    void close();
    bool createDB();

    bool startTransaction();
    bool commit();

    bool initializeInfoTable();
    bool createInfoTable();
    bool updateInfoTable(double version);

    bool createSeriesTable();
    bool createEpisodeTable();
    bool createBannerTable();

    bool insertSeries(int id, QString actors, QString airsDayOfWeek,
                      QString airsTime, QString contentRating,
                      QString firstAired, QString genre, QString imdb_id,
                      QString language, QString network, QString overview,
                      double rating, int ratingCount, int runtime, QString seriesName,
                      QString status, QString added, int addedby, QString banner,
                      QString fanart, QString lastUpdated, QString poster, QString zap2itid, int watched);
    bool insertEpisode(int id, QString director, int epimgflag, QString episodeName,
                       int episodeNumber, QString firstAired, QString guestStars, QString imdb_id,
                       QString language, QString overview, int productionCode, double rating,
                       int ratingCount, int seasonNumber, QString writer, int absoluteNumber,
                       int airsAfterSeason, int airsBeforeEpisode, int airsBeforeSeason , QString filename,
                       QString lastUpdated, int seasonID, int seriesID, QString thumbAdded,
                       int thumbHeight, int thumbWidth);
    bool insertBanner(int id, int seriesID, QString bannerPath, QString bannerType,
                      QString bannerType2, QString language, int season);

    bool updateSeries();
    bool updateEpisode();

    bool deleteSeries(int seriesID);
    bool deleteAllSeries();

    QList<QMap<QString, QString> > getSeries();
    QList<QList<QString> > getStartPageSeries();
    QList<QList<QString> > getEpisodes(int seriesID, int seasonNumber);
    QMap<QString, QString> getNextEpisodeDetails(int seriesID);
    QList<QString> getTodaysEpisodes(int seriesID);
    QString getStatus(int seriesID);
    QString getSeasonBanner(int seriesID, int season);

    void toggleWatched(QString episodeID);
    void markSeasonWatched(int seriesID, int season);

    bool isAlreadyAdded(int seriesID, QString name);
    int watchedCount(int seriesID);
    int watchedCountBySeason(int seriesID, int seasonNumber);
    int totalCount(int seriesID);
    int totalCountBySeason(int seriesID, int seasonNumber);
    int seasonCount(int seriesID);

signals:

public slots:

private:
    QSqlDatabase db;


};

#endif // DATABASEMANAGER_H
