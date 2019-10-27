#ifndef SERIESDATA_H
#define SERIESDATA_H

#include <QObject>
#include <QDebug>

class SeriesData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int ID READ getID CONSTANT)
    Q_PROPERTY(QString Actors READ getActors CONSTANT)
    Q_PROPERTY(QString AirsDayOfWeek READ getAirsDayOfWeek CONSTANT)
    Q_PROPERTY(QString AirsTime READ getAirsTime CONSTANT)
    Q_PROPERTY(QString ContentRating READ getContentRating CONSTANT)
    Q_PROPERTY(QString FirstAired READ getFirstAired CONSTANT)
    Q_PROPERTY(QString Genre READ getGenre CONSTANT)
    Q_PROPERTY(QString IMDB_ID READ getIMDB_ID CONSTANT)
    Q_PROPERTY(QString Language READ getLanguage CONSTANT)
    Q_PROPERTY(QString Network READ getNetwork CONSTANT)
    Q_PROPERTY(QString Overview READ getOverview CONSTANT)
    Q_PROPERTY(QString Rating READ getRating CONSTANT)
    Q_PROPERTY(int RatingCount READ getRatingCount CONSTANT)
    Q_PROPERTY(int Runtime READ getRuntime CONSTANT)
    Q_PROPERTY(QString SeriesName READ getSeriesName CONSTANT)
    Q_PROPERTY(QString AliasNames READ getAliasNames CONSTANT)
    Q_PROPERTY(QString Status READ getStatus CONSTANT)
    Q_PROPERTY(QString Added READ getAdded CONSTANT)
    Q_PROPERTY(int AddedBy READ getAddedBy CONSTANT)
    Q_PROPERTY(QString Banner READ getBanner CONSTANT)
    Q_PROPERTY(QString Fanart READ getFanart CONSTANT)
    Q_PROPERTY(QString LastUpdated READ getLastUpdated CONSTANT)
    Q_PROPERTY(QString Poster READ getPoster CONSTANT)
    Q_PROPERTY(QString zap2it_ID READ getZap2it_ID CONSTANT)
    Q_PROPERTY(int Watched READ getWatched WRITE setWatched NOTIFY watchedChanged)

    Q_PROPERTY(QString NextEpisodeId READ getNextEpisodeId CONSTANT)
    Q_PROPERTY(QString NextEpisodeName READ getNextEpisodeName CONSTANT)
    Q_PROPERTY(QString NextEpisodeNumber READ getNextEpisodeNumber CONSTANT)
    Q_PROPERTY(QString NextEpisodeSeasonNumber READ getNextEpisodeSeasonNumber CONSTANT)
    Q_PROPERTY(QString DaysToNextEpisode READ getDaysToNextEpisode CONSTANT)
    Q_PROPERTY(QString WatchedCount READ getWatchedCount CONSTANT)
    Q_PROPERTY(QString TotalCount READ getTotalCount CONSTANT)
    Q_PROPERTY(QString NextEpisodeFirstAired READ getNextEpisodeFirstAired CONSTANT)
    Q_PROPERTY(QString NextEpisodeBanner READ getNextEpisodeBanner CONSTANT)
    Q_PROPERTY(QString NextEpisodeOverview READ getNextEpisodeOverview CONSTANT)
    Q_PROPERTY(QString NextEpisodeGuestStars READ getNextEpisodeGuestStars CONSTANT)
    Q_PROPERTY(QString NextEpisodeWriter READ getNextEpisodeWriter CONSTANT)
    Q_PROPERTY(int NextEpisodeWatched READ getNextEpisodeWatched WRITE setNextEpisodeWatched NOTIFY nextEpisodeWatchedChanged)

public:
    explicit SeriesData(QObject *parent = 0, QVariantMap params = QVariantMap());

    int getID();
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
    int getNextEpisodeWatched();
    void setNextEpisodeWatched(int watched);

signals:

    void watchedChanged();
    void nextEpisodeWatchedChanged();

public slots:

private:

    int m_id;
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
    int m_nextEpisodeWatched;
};

#endif // SERIESDATA_H
