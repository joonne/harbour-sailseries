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
    Q_PROPERTY(QString Rating READ getRating)
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
    Q_PROPERTY(QString Poster READ getPoster)
    Q_PROPERTY(QString zap2it_ID READ getZap2it_ID)
    Q_PROPERTY(int Watched READ getWatched WRITE setWatched NOTIFY watchedChanged)

    Q_PROPERTY(QString NextEpisodeName READ getNextEpisodeName)
    Q_PROPERTY(QString NextEpisodeNumber READ getNextEpisodeNumber)
    Q_PROPERTY(QString NextEpisodeSeasonNumber READ getNextEpisodeSeasonNumber)
    Q_PROPERTY(QString DaysToNextEpisode READ getDaysToNextEpisode)
    Q_PROPERTY(QString WatchedCount READ getWatchedCount)
    Q_PROPERTY(QString TotalCount READ getTotalCount)

public:
    explicit SeriesData(QObject *parent = 0, QMap<QString, QString> params = QMap<QString, QString>());

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
    QString getRating();
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
    QString getPoster();
    QString getZap2it_ID();
    int getWatched();
    void setWatched(int watched);

    QString getNextEpisodeName();
    QString getNextEpisodeNumber();
    QString getNextEpisodeSeasonNumber();
    QString getDaysToNextEpisode();
    QString getWatchedCount();
    QString getTotalCount();

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
    QString myRating;
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
    QString myPoster;
    QString myZap2it_ID;
    int myWatched;

    QString myNextEpisodeName;
    QString myNextEpisodeNumber;
    QString myNextEpisodeSeasonNumber;
    QString myDaysToNextEpisode;
    QString myWatchedCount;
    QString myTotalCount;

};

#endif // SERIESDATA_H
