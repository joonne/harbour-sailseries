#ifndef EPISODEDATA_H
#define EPISODEDATA_H

#include <QObject>
#include <QDebug>

class EpisodeData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int ID READ getID CONSTANT)
    Q_PROPERTY(QString Director READ getDirector CONSTANT)
    Q_PROPERTY(int Epimgflag READ getEpimgflag CONSTANT)
    Q_PROPERTY(QString EpisodeName READ getEpisodeName CONSTANT)
    Q_PROPERTY(int EpisodeNumber READ getEpisodeNumber CONSTANT)
    Q_PROPERTY(QString FirstAired READ getFirstAired CONSTANT)
    Q_PROPERTY(QString GuestStars READ getGuestStars CONSTANT)
    Q_PROPERTY(QString IMDB_ID READ getIMDB_ID CONSTANT)
    Q_PROPERTY(QString Language READ getLanguage CONSTANT)
    Q_PROPERTY(QString Overview READ getOverview CONSTANT)
    Q_PROPERTY(int ProductionCode READ getProductionCode CONSTANT)
    Q_PROPERTY(double Rating READ getRating CONSTANT)
    Q_PROPERTY(int RatingCount READ getRatingCount CONSTANT)
    Q_PROPERTY(int SeasonNumber READ getSeasonNumber CONSTANT)
    Q_PROPERTY(QString Writer READ getWriter CONSTANT)
    Q_PROPERTY(int AbsoluteNumber READ getAbsoluteNumber CONSTANT)
    Q_PROPERTY(int AirsAfterSeason READ getAirsAfterSeason CONSTANT)
    Q_PROPERTY(int AirsBeforeSeason READ getAirsBeforeSeason CONSTANT)
    Q_PROPERTY(QString Filename READ getFilename CONSTANT)
    Q_PROPERTY(QString LastUpdated READ getLastUpdated CONSTANT)
    Q_PROPERTY(int SeasonID READ getSeasonID CONSTANT)
    Q_PROPERTY(int SeriesID READ getSeriesID CONSTANT)
    Q_PROPERTY(QString ThumbAdded READ getThumbAdded CONSTANT)
    Q_PROPERTY(int ThumbHeight READ getThumbHeight CONSTANT)
    Q_PROPERTY(int ThumbWidth READ getThumbWidth CONSTANT)
    Q_PROPERTY(int Watched READ getWatched WRITE setWatched NOTIFY watchedChanged)

public:
    explicit EpisodeData(QObject *parent = 0, QVariantMap params = QVariantMap());

    int getID();
    QString getDirector();
    int getEpimgflag();
    QString getEpisodeName();
    int getEpisodeNumber();
    QString getFirstAired();
    QString getGuestStars();
    QString getIMDB_ID();
    QString getLanguage();
    QString getOverview();
    int getProductionCode();
    double getRating();
    int getRatingCount();
    int getSeasonNumber();
    QString getWriter();
    int getAbsoluteNumber();
    int getAirsAfterSeason();
    int getAirsBeforeSeason();
    QString getFilename();
    QString getLastUpdated();
    int getSeasonID();
    int getSeriesID();
    QString getThumbAdded();
    int getThumbHeight();
    int getThumbWidth();
    bool getWatched();
    void setWatched(const bool &watched);

signals:
    void watchedChanged();

private:
    int m_id;
    QString m_director;
    int m_epimgflag;
    QString m_episodeName;
    int m_episodeNumber;
    QString m_firstAired;
    QString m_guestStars;
    QString m_imdbId;
    QString m_language;
    QString m_overview;
    int m_productionCode;
    double m_rating;
    int m_ratingCount;
    int m_seasonNumber;
    QString m_writer;
    int m_absoluteNumber;
    int m_airsAfterSeason;
    int m_airsBeforeSeason;
    QString m_filename;
    QString m_lastUpdated;
    int m_seasonId;
    int m_seriesId;
    QString m_thumbAdded;
    int m_thumbHeight;
    int m_thumbWidth;
    int m_watched;
    QString m_watchedFlagImage;
};

#endif // EPISODEDATA_H
