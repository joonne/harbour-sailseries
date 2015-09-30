#ifndef EPISODEDATA_H
#define EPISODEDATA_H

#include <QObject>
#include <QDebug>

class EpisodeData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int ID READ getID NOTIFY IDChanged)
    Q_PROPERTY(QString Director READ getDirector NOTIFY directorChanged)
    Q_PROPERTY(int Epimgflag READ getEpimgflag NOTIFY epimgflagChanged)
    Q_PROPERTY(QString EpisodeName READ getEpisodeName NOTIFY episodeNameChanged)
    Q_PROPERTY(int EpisodeNumber READ getEpisodeNumber NOTIFY episodeNumberChanged)
    Q_PROPERTY(QString FirstAired READ getFirstAired NOTIFY firstAiredChanged)
    Q_PROPERTY(QString GuestStars READ getGuestStars NOTIFY guestStarsChanged)
    Q_PROPERTY(QString IMDB_ID READ getIMDB_ID NOTIFY IMDB_IDChanged)
    Q_PROPERTY(QString Language READ getLanguage NOTIFY languageChanged)
    Q_PROPERTY(QString Overview READ getOverview NOTIFY overviewChanged)
    Q_PROPERTY(int ProductionCode READ getProductionCode NOTIFY productionCodeChanged)
    Q_PROPERTY(double Rating READ getRating NOTIFY ratingChanged)
    Q_PROPERTY(int RatingCount READ getRatingCount NOTIFY ratingCountChanged)
    Q_PROPERTY(int SeasonNumber READ getSeasonNumber NOTIFY seasonNumberChanged)
    Q_PROPERTY(QString Writer READ getWriter NOTIFY writerChanged)
    Q_PROPERTY(int AbsoluteNumber READ getAbsoluteNumber NOTIFY absoluteNumberChanged)
    Q_PROPERTY(int AirsAfterSeason READ getAirsAfterSeason NOTIFY airsAfterSeasonChanged)
    Q_PROPERTY(int AirsBeforeSeason READ getAirsBeforeSeason NOTIFY airsBeforeSeasonChanged)
    Q_PROPERTY(QString Filename READ getFilename NOTIFY filenameChanged)
    Q_PROPERTY(QString LastUpdated READ getLastUpdated NOTIFY lastUpdatedChanged)
    Q_PROPERTY(int SeasonID READ getSeasonID NOTIFY seasonIDChanged)
    Q_PROPERTY(int SeriesID READ getSeriesID NOTIFY seriesIDChanged)
    Q_PROPERTY(QString ThumbAdded READ getThumbAdded NOTIFY thumbAddedChanged)
    Q_PROPERTY(int ThumbHeight READ getThumbHeight NOTIFY thumbHeightChanged)
    Q_PROPERTY(int ThumbWidth READ getThumbWidth NOTIFY thumbWidthChanged)
    Q_PROPERTY(int Watched READ getWatched WRITE setWatched NOTIFY watchedChanged)
    Q_PROPERTY(QString WatchedFlagImage READ getWatchedFlagImage WRITE setWatchedFlagImage NOTIFY watchedFlagImageChanged)

public:
    explicit EpisodeData(QObject *parent = 0);
    EpisodeData(QObject* parent, int ID, QString director, int epimgflag, QString episodeName,
                int episodeNumber, QString firstAired, QString guestStars, QString IMDB_ID,
                QString language, QString overview, int productionCode, double rating,
                int ratingCount, int SeasonNumber, QString writer, int absoluteNumber,
                int airsAfterSeason, int airsBeforeSeason, QString filename, QString lastUpdated,
                int seasonID, int seriesID, QString thumbAdded, int thumbHeight, int thumbWidth,
                int watched);
    EpisodeData(QObject *parent, QString episodeName, int episodeNumber, QString overview,
                int seasonNumber, int absoluteNumber, QString filename, int watched, int id,
                QString guestStars, QString writer);

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
    int getWatched();
    void setWatched(int watched);
    QString getWatchedFlagImage();
    void setWatchedFlagImage(QString imagepath);

signals:
    void IDChanged();
    void directorChanged();
    void epimgflagChanged();
    void episodeNameChanged();
    void episodeNumberChanged();
    void firstAiredChanged();
    void guestStarsChanged();
    void IMDB_IDChanged();
    void languageChanged();
    void overviewChanged();
    void productionCodeChanged();
    void ratingChanged();
    void ratingCountChanged();
    void seasonNumberChanged();
    void writerChanged();
    void absoluteNumberChanged();
    void airsAfterSeasonChanged();
    void airsBeforeSeasonChanged();
    void filenameChanged();
    void lastUpdatedChanged();
    void seasonIDChanged();
    void seriesIDChanged();
    void thumbAddedChanged();
    void thumbHeightChanged();
    void thumbWidthChanged();
    void watchedChanged();
    void watchedFlagImageChanged();

public slots:

private:

    int myID;
    QString myDirector;
    int myEpimgflag;
    QString myEpisodeName;
    int myEpisodeNumber;
    QString myFirstAired;
    QString myGuestStars;
    QString myIMDB_ID;
    QString myLanguage;
    QString myOverview;
    int myProductionCode;
    double myRating;
    int myRatingCount;
    int mySeasonNumber;
    QString myWriter;
    int myAbsoluteNumber;
    int myAirsAfterSeason;
    int myAirsBeforeSeason;
    QString myFilename;
    QString myLastUpdated;
    int mySeasonID;
    int mySeriesID;
    QString myThumbAdded;
    int myThumbHeight;
    int myThumbWidth;
    int myWatched;
    QString myWatchedFlagImage;
};

#endif // EPISODEDATA_H
