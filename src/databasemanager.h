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
#include "xmlreader.h"
#include <QStringRef>

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
    bool updateDB();

    bool initializeInfoTable();
    bool createInfoTable();
    bool updateInfoTable(double version);

    bool createSeriesTable();
    bool createEpisodeTable();

    bool insertSeries(int id, QString actors, QString airsDayOfWeek,
                      QString airsTime, QString contentRating,
                      QString firstAired, QString genre, QString imdb_id,
                      QString language, QString network, QString overview,
                      double rating, int ratingCount, int runtime, QString SeriesName,
                      QString status, QString added, int addedby, QString banner,
                      QString fanart, QString lastUpdated, QString poster, QString zap2itid, int watched);
    bool insertEpisode(int id, QString director, int epimgflag, QString episodeName,
                       int episodeNumber, QString firstAired, QString guestStars, QString imdb_id,
                       QString language, QString overview, int productionCode, double rating,
                       int ratingCount, int seasonNumber, QString writer, int absoluteNumber,
                       int airsAfterSeason, int airsBeforeEpisode, int airsBeforeSeason , QString filename,
                       QString lastUpdated, int seasonID, int seriesID, QString thumbAdded,
                       int thumbHeight, int thumbWidth, int watched);
    bool insertEpisode();

    bool updateSeries();
    bool updateEpisode();

    QList<QList<QString> > getSeries();
    QList<QList<QString> > getStartPageSeries();

signals:

public slots:

private:
    QSqlDatabase db;
    XMLReader* myReader;


};

#endif // DATABASEMANAGER_H
