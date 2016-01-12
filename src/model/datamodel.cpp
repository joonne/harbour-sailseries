#include "datamodel.h"

DataModel::DataModel(QObject *parent) :
    QObject(parent),
    myReader(new XMLReader),
    mydbmanager(new DatabaseManager)
{
    mydbmanager->setUpDB();

    mySeriesListModel = new SeriesListModel(this, 0, mydbmanager, myReader);
    mySearchListModel = new SearchListModel(this, mydbmanager, myReader);
    myTodayListModel = new TodayListModel(this, mydbmanager, myReader);
    myEpisodeListModel = new EpisodeListModel(this, mydbmanager);
    mySeasonListModel = new SeasonListModel(this, mydbmanager);

    connect(mySearchListModel,
            SIGNAL(updateModels()),
            this,
            SLOT(readyToUpdateModels()));

    connect(mySeriesListModel,
            SIGNAL(updateModels()),
            this,
            SLOT(readyToUpdateModels()));

}

DataModel::~DataModel() {

    delete myReader;
    delete mydbmanager;

    qDebug() << "destructing DataModel";

}

ProgramListModel* DataModel::getProgramListModel() { return myProgramListModel; }

SeriesListModel* DataModel::getSeriesListModel() { return mySeriesListModel; }

SearchListModel *DataModel::getSearchModel() { return mySearchListModel; }

TodayListModel* DataModel::getTodayModel() { return myTodayListModel; }

EpisodeListModel* DataModel::getEpisodeListModel() { return myEpisodeListModel; }

SeasonListModel* DataModel::getSeasonListModel() { return mySeasonListModel; }

void DataModel::readyToUpdateModels() {
    myTodayListModel->populateTodayModel();
    mySeriesListModel->populateBannerList();
    mySeriesListModel->populateSeriesList();
}
