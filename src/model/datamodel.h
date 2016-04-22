#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include "serieslistmodel.h"
#include "../xmlreader.h"
#include "../databasemanager.h"
#include "episodelistmodel.h"
#include "searchlistmodel.h"
#include "todaylistmodel.h"
#include "seasonlistmodel.h"

class DataModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SeriesListModel* SeriesListModel READ getSeriesListModel NOTIFY seriesListModelChanged)
    Q_PROPERTY(SearchListModel* SearchModel READ getSearchModel NOTIFY searchModelChanged)
    Q_PROPERTY(TodayListModel* TodayModel READ getTodayModel NOTIFY todayModelChanged)
    Q_PROPERTY(EpisodeListModel* EpisodeListModel READ getEpisodeListModel NOTIFY episodeListModelChanged)
    Q_PROPERTY(SeasonListModel* SeasonListModel READ getSeasonListModel NOTIFY seasonListModelChanged)

public:
    explicit DataModel(QObject *parent = 0);
    ~DataModel();

    SeriesListModel *getSeriesListModel();
    SearchListModel* getSearchModel();
    TodayListModel *getTodayModel();
    EpisodeListModel* getEpisodeListModel();
    SeasonListModel* getSeasonListModel();

signals:
    void seriesListModelChanged();
    void programListModelChanged();
    void searchModelChanged();
    void todayModelChanged();
    void episodeListModelChanged();
    void seasonListModelChanged();

public slots:
    void readyToUpdateModels();

private:
    SeriesListModel* mySeriesListModel;
    SearchListModel* mySearchListModel;
    TodayListModel* myTodayListModel;
    EpisodeListModel* myEpisodeListModel;
    SeasonListModel* mySeasonListModel;
    XMLReader* myReader;
    DatabaseManager* mydbmanager;
};

#endif // DATAMODEL_H
