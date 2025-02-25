#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QThread>
#include "api.h"
#include "databasemanager.h"
#include "model/serieslistmodel.h"
#include "model/episodelistmodel.h"
#include "model/searchlistmodel.h"
#include "model/seasonlistmodel.h"

class Engine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SeriesListModel* SeriesListModel READ getSeriesListModel NOTIFY seriesListModelChanged)
    Q_PROPERTY(SearchListModel* SearchModel READ getSearchModel NOTIFY searchModelChanged)
    Q_PROPERTY(EpisodeListModel* EpisodeListModel READ getEpisodeListModel NOTIFY episodeListModelChanged)
    Q_PROPERTY(SeasonListModel* SeasonListModel READ getSeasonListModel NOTIFY seasonListModelChanged)
    Q_PROPERTY(bool Loading READ getLoading NOTIFY loadingChanged)
    Q_PROPERTY(QString Mode READ getMode NOTIFY modeChanged)

public:
    explicit Engine(QObject *parent = 0);
    ~Engine();

    SeriesListModel *getSeriesListModel();
    SearchListModel* getSearchModel();
    EpisodeListModel* getEpisodeListModel();
    SeasonListModel* getSeasonListModel();
    bool getLoading();
    QString getMode();

    Q_INVOKABLE void updateModels();
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

public slots:
    void readyToUpdateModels();
    void setLoading(const bool &state);
    void seriesStored(const int &seriesId);
    void setMode(const QString &mode);

private:
    SeriesListModel* m_seriesListModel;
    SearchListModel* m_searchListModel;
    EpisodeListModel* m_episodeListModel;
    SeasonListModel* m_seasonListModel;
    Api* m_api;
    DatabaseManager* m_dbmanager;
    bool m_loading;
    QString m_mode;
};

#endif // ENGINE_H
