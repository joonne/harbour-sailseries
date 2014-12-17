#ifndef SERIESDATA_H
#define SERIESDATA_H

#include <QObject>
#include <QDebug>

class SeriesData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString ID READ getID)
    Q_PROPERTY(QString Actors READ getActors)
    Q_PROPERTY(QString AirsDayOfWeek READ getAirsDayOfWeek)
    Q_PROPERTY(QString AirsTime READ getAirsTime)
    Q_PROPERTY(QString ContentRating READ getContentRating)
    Q_PROPERTY(QString FirstAired READ getFirstAired)
    Q_PROPERTY(QString Genre READ getGenre)
    Q_PROPERTY(QString IMDB_ID READ getIMDB_ID)
    Q_PROPERTY(QString Language READ getLanguage)
    Q_PROPERTY(QString Network READ getNetwork)
    Q_PROPERTY(QString Overview READ getOverview)
    Q_PROPERTY(double Rating READ getRating)
    Q_PROPERTY(int RatingCount READ getRatingCount)
    Q_PROPERTY(int Runtime READ getRuntime)
    Q_PROPERTY(QString SeriesName READ getSeriesName)
    Q_PROPERTY(QString AliasNames READ getAliasNames)
    Q_PROPERTY(QString Status READ getStatus)
    Q_PROPERTY(QString Added READ getAdded)
    Q_PROPERTY(int AddedBy READ getAddedBy)
    Q_PROPERTY(QString Banner READ getBanner)
    Q_PROPERTY(QString Fanart READ getFanart)
    Q_PROPERTY(QString LastUpdated READ getLastUpdated)
    Q_PROPERTY(QString Posters READ getPosters)
    Q_PROPERTY(QString zap2it_ID READ getZap2it_ID)
    Q_PROPERTY(int Watched READ getWatched WRITE setWatched NOTIFY watchedChanged)

public:
    explicit SeriesData(QObject *parent = 0);

    SeriesData(QObject* parent, QString id, QString language, QString seriesName,
               QString aliasNames, QString banner, QString overview, QString firstAired, QString IMDB_ID,
               QString zap2it_ID, QString network);

    SeriesData(QObject* parent, QString id, QString actors, QString airsDayOfWeek,
               QString airsTime, QString contentRating,
               QString firstAired, QString genre, QString IMDB_ID,
               QString language, QString network, QString overview,
               double rating, int ratingCount, int runtime, QString seriesName,
               QString status, QString added, int addedby, QString banner,
               QString fanart, QString lastUpdated, QString posters, QString zap2itid, int watched);

    SeriesData(QObject* parent, QString banner, QString poster);

    SeriesData(QObject* parent, QString seriesName, QString network, QString airsTime);

    QString getID();
    QString getActors();
    QString getAirsDayOfWeek();
    QString getAirsTime();
    QString getContentRating();
    QString getFirstAired();
    QString getGenre();
    QString getIMDB_ID();
    QString getLanguage();
    QString getNetwork();
    QString getOverview();
    double getRating();
    int getRatingCount();
    int getRuntime();
    QString getSeriesName();
    QString getAliasNames();
    QString getStatus();
    QString getAdded();
    int getAddedBy();
    QString getBanner();
    QString getFanart();
    QString getLastUpdated();
    QString getPosters();
    QString getZap2it_ID();
    int getWatched();


    void setWatched(int watched);

signals:

    void watchedChanged();

public slots:

private:

    QString myID;
    QString myActors;
    QString myAirsDayOfWeek;
    QString myAirsTime;
    QString myContentRating;
    QString myFirstAired;
    QString myGenre;
    QString myIMDB_ID;
    QString myLanguage;
    QString myNetwork;
    QString myOverview;
    double myRating;
    int myRatingCount;
    int myRuntime;
    QString mySeriesName;
    QString myAliasNames;
    QString myStatus;
    QString myAdded;
    int myAddedBy;
    QString myBanner;
    QString myFanart;
    QString myLastUpdated;
    QString myPosters;
    QString myZap2it_ID;
    int myWatched;

};

#endif // SERIESDATA_H
