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
    Q_PROPERTY(QString Poster READ getPoster WRITE setPoster NOTIFY posterChanged)
    Q_PROPERTY(QString Mode READ getMode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(QString NextEpisodeName READ getNextEpisodeName)
    Q_PROPERTY(QString NextEpisodeNumber READ getNextEpisodeNumber)
    Q_PROPERTY(QString NextEpisodeSeasonNumber READ getNextEpisodeSeasonNumber)
    Q_PROPERTY(QString DaysToNextEpisode READ getDaysToNextEpisode)
    Q_PROPERTY(QString Status READ getStatus)
    Q_PROPERTY(QString Rating READ getRating)


public:
    explicit SeriesListModel(QObject *parent = 0, QQmlContext* context = 0, DatabaseManager *dbmanager = 0, XMLReader* reader = 0);
    ~SeriesListModel();

    void populateSeriesList();
    Q_INVOKABLE void populateBannerList();
    Q_INVOKABLE void selectSeries(int index);
    Q_INVOKABLE void nextPoster();

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

    QString getPoster();
    void setPoster(QString);

    bool getLoading();
    void setLoading(bool);

    QString getMode();
    void setMode(QString newmode);

    void storeSeries();
    void storeEpisodes();
    Q_INVOKABLE void deleteSeries(int seriesID);
    Q_INVOKABLE void updateSeries(QString seriesID);


signals:
    void seriesListChanged();
    void loadingChanged();
    void posterChanged();
    void modeChanged();
    void updateModels();

public slots:
    void updateFetchFinished();

private:
    XMLReader* myReader;
    DatabaseManager* mydbmanager;
    QList<QMap<QString,QString> > mySeries;
    QList<QMap<QString,QString> > myEpisodes;
    QQmlContext* myContext;
    QList<SeriesData*> mySeriesListModel;
    bool isPopulated;
    SeriesData* myInfo;

    static int seriesListCount(QQmlListProperty<SeriesData> *prop);
    static SeriesData* seriesListAt(QQmlListProperty<SeriesData> *prop, int index);
    static void seriesListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val);
    static void seriesListClear(QQmlListProperty<SeriesData>* prop);

    bool myLoading;

    QList<QString> myPosters;
    int posterIndex;

    QString mode;

};

#endif // SERIESLISTMODEL_H
