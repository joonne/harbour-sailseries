#ifndef SERIESLISTMODEL_H
#define SERIESLISTMODEL_H

#include <QObject>
#include <QQmlContext>
#include <QDebug>
#include <QQmlListProperty>

#include "seriesdata.h"
#include "episodedata.h"
#include "../xmlreader.h"
#include "../databasemanager.h"

class SeriesListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<SeriesData> SeriesList READ getSeriesList NOTIFY seriesListChanged)
    Q_PROPERTY(QString ID READ getID)
    Q_PROPERTY(QString Language READ getLanguage)
    Q_PROPERTY(QString SeriesName READ getSeriesName)
    Q_PROPERTY(QString AliasNames READ getAliasNames)
    Q_PROPERTY(QString Banner READ getBanner)
    Q_PROPERTY(QString Overview READ getOverview)
    Q_PROPERTY(QString FirstAired READ getFirstAired)
    Q_PROPERTY(QString IMDB_ID READ getIMDB_ID)
    Q_PROPERTY(QString zap2it_ID READ getZap2it_ID)
    Q_PROPERTY(QString Network READ getNetwork)
    Q_PROPERTY(bool Loading READ getLoading WRITE setLoading NOTIFY loadingChanged)
    Q_PROPERTY(QString Poster READ getPoster NOTIFY posterChanged)
    Q_PROPERTY(QString Mode READ getMode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(QString NextEpisodeName READ getNextEpisodeName NOTIFY nextEpisodeNameChanged)
    Q_PROPERTY(QString NextEpisodeNumber READ getNextEpisodeNumber NOTIFY nextEpisodeNumberChanged)
    Q_PROPERTY(QString NextEpisodeSeasonNumber READ getNextEpisodeSeasonNumber NOTIFY nextEpisodeSeasonNumberChanged)
    Q_PROPERTY(QString DaysToNextEpisode READ getDaysToNextEpisode NOTIFY daysToNextEpisodeChanged)
    Q_PROPERTY(QString Status READ getStatus NOTIFY statusChanged)
    Q_PROPERTY(QString Rating READ getRating NOTIFY ratingChanged)
    Q_PROPERTY(QString Genre READ getGenre NOTIFY genreChanged)


public:
    explicit SeriesListModel(QObject *parent = 0, DatabaseManager *dbmanager = 0, XMLReader* reader = 0);
    ~SeriesListModel();

    Q_INVOKABLE void populateBannerList();
    Q_INVOKABLE void selectSeries(int index);

    QQmlListProperty<SeriesData> getSeriesList();
    QString getID();
    QString getLanguage();
    QString getSeriesName();
    QString getAliasNames();
    QString getBanner();
    QString getOverview();
    QString getFirstAired();
    QString getIMDB_ID();
    QString getZap2it_ID();
    QString getNetwork();
    QString getNextEpisodeName();
    QString getNextEpisodeNumber();
    QString getNextEpisodeSeasonNumber();
    QString getDaysToNextEpisode();
    QString getStatus();
    QString getRating();
    QString getGenre();

    bool getLoading();
    void setLoading(bool);

    QString getPoster();

    QString getMode();
    void setMode(QString newmode);

    void storeSeries();
    void storeEpisodes();
    void storeBanners();
    Q_INVOKABLE void deleteSeries(int seriesID);
    Q_INVOKABLE void updateSeries(QString seriesId = "");
    Q_INVOKABLE void updateAllSeries();

signals:
    void seriesListChanged();
    void loadingChanged();
    void posterChanged();
    void modeChanged();
    void updateModels();
    void nextEpisodeNameChanged();
    void nextEpisodeNumberChanged();
    void nextEpisodeSeasonNumberChanged();
    void daysToNextEpisodeChanged();
    void statusChanged();
    void ratingChanged();
    void genreChanged();

public slots:
    void updateFetchFinished();

private:
    XMLReader* myReader;
    DatabaseManager* mydbmanager;
    QList<QMap<QString,QString> > mySeries;
    QList<QMap<QString,QString> > myEpisodes;
    QList<QMap<QString,QString> > myBanners;
    QQmlContext* myContext;
    QList<SeriesData*> mySeriesListModel;
    SeriesData* myInfo;
    QList<QString> mySeriesIds;

    static int seriesListCount(QQmlListProperty<SeriesData> *prop);
    static SeriesData* seriesListAt(QQmlListProperty<SeriesData> *prop, int index);
    static void seriesListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val);
    static void seriesListClear(QQmlListProperty<SeriesData>* prop);

    bool myLoading;

    QString myPoster;
    QString myNextEpisodeName;
    QString myNextEpisodeNumber;
    QString myNextEpisodeSeasonNumber;
    QString myDaysToNextEpisode;

    QString mode;

};

#endif // SERIESLISTMODEL_H
