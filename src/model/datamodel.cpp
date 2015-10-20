#include "datamodel.h"

DataModel::DataModel(QObject *parent) :
    QObject(parent),
    myReader(new XMLReader),
    mydbmanager(new DatabaseManager)
{
    mydbmanager->setUpDB();

    mySeriesListModel = new SeriesListModel(this,0,mydbmanager,myReader);
    mySearchListModel = new SearchListModel(this,mydbmanager,myReader);
    myTodayListModel = new TodayListModel(this,mydbmanager,myReader);
    myEpisodeListModel = new EpisodeListModel(this,mydbmanager);

    //myProgramListModel = new ProgramListModel(this,mydbmanager,myReader);
    //myReader->updateTVGuide();

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

void DataModel::readyToProcessEvent() {

}

ProgramListModel* DataModel::getProgramListModel() { return myProgramListModel; }

SeriesListModel* DataModel::getSeriesListModel() { return mySeriesListModel; }

SearchListModel *DataModel::getSearchModel() { return mySearchListModel; }

TodayListModel* DataModel::getTodayModel() { return myTodayListModel; }

EpisodeListModel* DataModel::getEpisodeListModel() { return myEpisodeListModel; }

void DataModel::readyToUpdateModels() {
    myTodayListModel->populateTodayModel();
}
