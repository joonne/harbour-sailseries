#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include "api.h"
#include <QThread>
#include "databasemanager.h"
#include "statistics.h"
#include "./model/serieslistmodel.h"
#include "./model/episodelistmodel.h"
#include "./model/searchlistmodel.h"
#include "./model/todaylistmodel.h"
#include "./model/seasonlistmodel.h"

class Engine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SeriesListModel* SeriesListModel READ getSeriesListModel NOTIFY seriesListModelChanged)
    Q_PROPERTY(SearchListModel* SearchModel READ getSearchModel NOTIFY searchModelChanged)
    Q_PROPERTY(TodayListModel* TodayModel READ getTodayModel NOTIFY todayModelChanged)
    Q_PROPERTY(EpisodeListModel* EpisodeListModel READ getEpisodeListModel NOTIFY episodeListModelChanged)
    Q_PROPERTY(SeasonListModel* SeasonListModel READ getSeasonListModel NOTIFY seasonListModelChanged)
    Q_PROPERTY(bool Loading READ getLoading NOTIFY loadingChanged)
    Q_PROPERTY(QString Mode READ getMode NOTIFY modeChanged)
    Q_PROPERTY(Statistics* Statistics READ getStatistics)

public:
    explicit Engine(QObject *parent = 0);
    ~Engine();

    SeriesListModel *getSeriesListModel();
    SearchListModel* getSearchModel();
    TodayListModel *getTodayModel();
    EpisodeListModel* getEpisodeListModel();
    SeasonListModel* getSeasonListModel();
    Statistics* getStatistics();
    bool getLoading();
    QString getMode();

    Q_INVOKABLE void updateModels();
    Q_INVOKABLE void deleteDuplicateEpisodes();
    Q_INVOKABLE void resetMode();

signals:
    void seriesListModelChanged();
    void programListModelChanged();
    void searchModelChanged();
    void todayModelChanged();
    void episodeListModelChanged();
    void seasonListModelChanged();
    void loadingChanged();
    void modeChanged();
    void setUpDb();
    void getEpisodeDetails(int);
    void deleteDuplicateEpisodesRequested();

public slots:
    void readyToUpdateModels();
    void setLoading(const bool &state);
    void seriesStored(const int &seriesId);
    void setMode(const QString &mode);

private:
    SeriesListModel* m_seriesListModel;
    SearchListModel* m_searchListModel;
    TodayListModel* m_todayListModel;
    EpisodeListModel* m_episodeListModel;
    SeasonListModel* m_seasonListModel;
    Api* m_api;
    DatabaseManager* m_dbmanager;
    Statistics* m_statistics;
    bool m_loading;
    QString m_mode;
};

#endif // ENGINE_H
