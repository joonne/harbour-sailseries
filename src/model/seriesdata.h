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

    Q_PROPERTY(QString NextEpisodeId READ getNextEpisodeId)
    Q_PROPERTY(QString NextEpisodeName READ getNextEpisodeName)
    Q_PROPERTY(QString NextEpisodeNumber READ getNextEpisodeNumber)
    Q_PROPERTY(QString NextEpisodeSeasonNumber READ getNextEpisodeSeasonNumber)
    Q_PROPERTY(QString DaysToNextEpisode READ getDaysToNextEpisode)
    Q_PROPERTY(QString WatchedCount READ getWatchedCount)
    Q_PROPERTY(QString TotalCount READ getTotalCount)
    Q_PROPERTY(QString NextEpisodeFirstAired READ getNextEpisodeFirstAired())
    Q_PROPERTY(QString NextEpisodeBanner READ getNextEpisodeBanner)
    Q_PROPERTY(QString NextEpisodeOverview READ getNextEpisodeOverview)
    Q_PROPERTY(QString NextEpisodeGuestStars READ getNextEpisodeGuestStars)
    Q_PROPERTY(QString NextEpisodeWriter READ getNextEpisodeWriter)

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

    QString getNextEpisodeId();
    QString getNextEpisodeName();
    QString getNextEpisodeNumber();
    QString getNextEpisodeSeasonNumber();
    QString getDaysToNextEpisode();
    QString getWatchedCount();
    QString getTotalCount();
    QString getNextEpisodeFirstAired();
    QString getNextEpisodeBanner();
    QString getNextEpisodeOverview();
    QString getNextEpisodeGuestStars();
    QString getNextEpisodeWriter();

signals:

    void watchedChanged();

public slots:

private:

    QString m_id;
    QString m_actors;
    QString m_airsDayOfWeek;
    QString m_airsTime;
    QString m_contentRating;
    QString m_firstAired;
    QString m_genre;
    QString m_imdbId;
    QString m_language;
    QString m_network;
    QString m_overview;
    QString m_rating;
    int m_ratingCount;
    int m_runtime;
    QString m_seriesName;
    QString m_aliasNames;
    QString m_status;
    QString m_added;
    int m_addedBy;
    QString m_banner;
    QString m_fanart;
    QString m_lastUpdated;
    QString m_poster;
    QString m_zap2itId;
    int m_watched;

    QString m_nextEpisodeName;
    QString m_nextEpisodeNumber;
    QString m_nextEpisodeSeasonNumber;
    QString m_daysToNextEpisode;

    QString m_watchedCount;
    QString m_totalCount;

    QString m_nextEpisodeId;
    QString m_nextEpisodeFirstAired;
    QString m_nextEpisodeBanner;
    QString m_nextEpisodeOverview;
    QString m_nextEpisodeGuestStars;
    QString m_nextEpisodeWriter;

};

#endif // SERIESDATA_H
