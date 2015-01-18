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

    connect(myReader,
            SIGNAL(readyToPopulateChannels()),
            this,
            SLOT(xmlParseFinished()));

    //myReader->updateTVGuide();

}

DataModel::~DataModel() {

    delete myReader;
    delete mydbmanager;

}

ProgramListModel* DataModel::getProgramListModel() { return myProgramListModel; }

SeriesListModel* DataModel::getSeriesListModel() { return mySeriesListModel; }

SearchListModel *DataModel::getSearchModel() { return mySearchListModel; }

TodayListModel* DataModel::getTodayModel() { return myTodayListModel; }

EpisodeListModel* DataModel::getEpisodeListModel() { return myEpisodeListModel; }

void DataModel::xmlParseFinished() {

    qDebug("tv-guiden slotti toimii");
    myPrograms = myReader->getTVGuide();
    myProgramListModel->populateChannel();

}
