#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include "api.h"
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

    Q_INVOKABLE void updateModels();
    bool getLoading();

    Q_INVOKABLE bool deleteDuplicateEpisodes();

signals:
    void seriesListModelChanged();
    void programListModelChanged();
    void searchModelChanged();
    void todayModelChanged();
    void episodeListModelChanged();
    void seasonListModelChanged();
    void loadingChanged();

public slots:
    void readyToUpdateModels();

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
    void toggleLoading(bool state);
};

#endif // ENGINE_H
