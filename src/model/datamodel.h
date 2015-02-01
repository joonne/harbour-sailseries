#ifndef DATAMODEL_H
#define DATAMODEL_H

#include <QObject>
#include "programlistmodel.h"
#include "serieslistmodel.h"
#include "../xmlreader.h"
#include "../databasemanager.h"
#include "episodelistmodel.h"
#include "searchlistmodel.h"
#include "todaylistmodel.h"

class DataModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(ProgramListModel* ProgramListModel READ getProgramListModel NOTIFY programListModelChanged)
    Q_PROPERTY(SeriesListModel* SeriesListModel READ getSeriesListModel NOTIFY seriesListModelChanged)
    Q_PROPERTY(SearchListModel* SearchModel READ getSearchModel NOTIFY searchModelChanged)
    Q_PROPERTY(TodayListModel* TodayModel READ getTodayModel NOTIFY todayModelChanged)
    Q_PROPERTY(EpisodeListModel* EpisodeListModel READ getEpisodeListModel NOTIFY episodeListModelChanged)

public:
    explicit DataModel(QObject *parent = 0);
    ~DataModel();

    ProgramListModel* getProgramListModel();
    SeriesListModel *getSeriesListModel();
    SearchListModel* getSearchModel();
    TodayListModel *getTodayModel();
    EpisodeListModel* getEpisodeListModel();

signals:
    void seriesListModelChanged();
    void programListModelChanged();
    void searchModelChanged();
    void todayModelChanged();
    void episodeListModelChanged();

public slots:
    void readyToUpdateModels();

private:
    ProgramListModel* myProgramListModel;
    SeriesListModel* mySeriesListModel;
    SearchListModel* mySearchListModel;
    TodayListModel* myTodayListModel;
    EpisodeListModel* myEpisodeListModel;
    XMLReader* myReader;
    DatabaseManager* mydbmanager;

//    QMap<QString,ProgramListModel*> channels;
//    QList<QMap<QString,QList<QMap<QString,QString> > > > myPrograms;


};

#endif // DATAMODEL_H
