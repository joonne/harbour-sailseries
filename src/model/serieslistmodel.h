#ifndef SERIESLISTMODEL_H
#define SERIESLISTMODEL_H

#include <QObject>
#include <QDebug>
#include <QQmlListProperty>

#include "seriesdata.h"
#include "episodedata.h"
#include "../api.h"
#include "../databasemanager.h"

class SeriesListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<SeriesData> SeriesList READ getSeriesList NOTIFY seriesListChanged)
    Q_PROPERTY(int ID READ getID CONSTANT)
    Q_PROPERTY(QString Language READ getLanguage CONSTANT)
    Q_PROPERTY(QString SeriesName READ getSeriesName CONSTANT)
    Q_PROPERTY(QString AliasNames READ getAliasNames CONSTANT)
    Q_PROPERTY(QString Banner READ getBanner CONSTANT)
    Q_PROPERTY(QString Overview READ getOverview CONSTANT)
    Q_PROPERTY(QString FirstAired READ getFirstAired CONSTANT)
    Q_PROPERTY(QString IMDB_ID READ getIMDB_ID CONSTANT)
    Q_PROPERTY(QString zap2it_ID READ getZap2it_ID CONSTANT)
    Q_PROPERTY(QString Network READ getNetwork CONSTANT)
    Q_PROPERTY(QString Poster READ getPoster NOTIFY posterChanged)
    Q_PROPERTY(QString Mode READ getMode WRITE setMode NOTIFY modeChanged)
    Q_PROPERTY(QString NextEpisodeName READ getNextEpisodeName NOTIFY nextEpisodeNameChanged)
    Q_PROPERTY(QString NextEpisodeNumber READ getNextEpisodeNumber NOTIFY nextEpisodeNumberChanged)
    Q_PROPERTY(QString NextEpisodeSeasonNumber READ getNextEpisodeSeasonNumber NOTIFY nextEpisodeSeasonNumberChanged)
    Q_PROPERTY(QString DaysToNextEpisode READ getDaysToNextEpisode NOTIFY daysToNextEpisodeChanged)
    Q_PROPERTY(QString Status READ getStatus CONSTANT)
    Q_PROPERTY(QString Rating READ getRating CONSTANT)
    Q_PROPERTY(QString Genre READ getGenre CONSTANT)

public:
    explicit SeriesListModel(QObject *parent = 0, Api *api = 0, DatabaseManager *dbmanager = 0);
    ~SeriesListModel();

    Q_INVOKABLE void populate();
    Q_INVOKABLE void selectSeries(const int &index);
    Q_INVOKABLE void deleteSeries(const int &seriesId);
    Q_INVOKABLE void updateSeries(const int &seriesId);
    Q_INVOKABLE void updateAllSeries(const bool &includeEndedSeries = true);

    QQmlListProperty<SeriesData> getSeriesList();
    int getID();
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
    QString getPoster();
    QString getMode();
    void setMode(QString mode);

signals:
    void seriesListChanged();
    void posterChanged();
    void modeChanged();
    void updateModels();
    void nextEpisodeNameChanged();
    void nextEpisodeNumberChanged();
    void nextEpisodeSeasonNumberChanged();
    void daysToNextEpisodeChanged();

    void getSeries();
    void deleteSeriesWith(int);
    void getAllRequested(int);
    void getSeriesIds(bool);
    void setLoading(bool);

public slots:
    void populateBannerList(const QList<QVariantMap> &allSeries);
    void seriesDeleted();
    void seriesStored();

private:
    Api* m_api;
    DatabaseManager* m_dbmanager;
    QList<SeriesData*> m_seriesListModel;
    SeriesData* m_info;
    QString m_mode;

    static int seriesListCount(QQmlListProperty<SeriesData> *prop);
    static SeriesData* seriesListAt(QQmlListProperty<SeriesData> *prop, int index);
    static void seriesListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val);
    static void seriesListClear(QQmlListProperty<SeriesData>* prop);
};

#endif // SERIESLISTMODEL_H
