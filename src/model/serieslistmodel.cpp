#include "serieslistmodel.h"

SeriesListModel::SeriesListModel(QObject *parent, QQmlContext* context, DatabaseManager* dbmanager, XMLReader *reader) :
    QObject(parent),
    myContext(context){

    myReader = reader;
    mydbmanager = dbmanager;

    mode = "default";

    connect(myReader,
            SIGNAL(readyToUpdateSeries()),
            this,
            SLOT(updateFetchFinished()));

    populateBannerList();
    populateSeriesList();

    myLoading = false;

}

SeriesListModel::~SeriesListModel() {

    foreach(SeriesData* series, mySeriesListModel) {
        delete series;
        series = 0;
    }

    qDebug() << "destructing SeriesListModel";
}

void SeriesListModel::updateFetchFinished() {

    storeSeries();
    storeEpisodes();
    storeBanners();
    setLoading(false);
    populateBannerList();
}

QQmlListProperty<SeriesData> SeriesListModel::getSeriesList() {

    return QQmlListProperty<SeriesData>(this,&mySeriesListModel,&SeriesListModel::seriesListCount,&SeriesListModel::seriesListAt);

}

void SeriesListModel::seriesListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val)
{
    SeriesListModel* seriesModel = qobject_cast<SeriesListModel*>(prop->object);
    seriesModel->mySeriesListModel.append(val);
}

SeriesData* SeriesListModel::seriesListAt(QQmlListProperty<SeriesData>* prop, int index)
{
    return (qobject_cast<SeriesListModel*>(prop->object))->mySeriesListModel.at(index);
}

int SeriesListModel::seriesListCount(QQmlListProperty<SeriesData>* prop)
{
    return qobject_cast<SeriesListModel*>(prop->object)->mySeriesListModel.size();
}

void SeriesListModel::seriesListClear(QQmlListProperty<SeriesData>* prop)
{
    qobject_cast<SeriesListModel*>(prop->object)->mySeriesListModel.clear();
}

// -------------------------------------------------------------------
// POPULATING DATA

void SeriesListModel::populateSeriesList() {

    if(mySeries.size() != 0) {

        int length = mySeries.size();
        for(int i = 0; i < length; ++i) {
            QMap<QString,QString> temp = mySeries.at(i);
            SeriesData* serie = new SeriesData(this, temp);
            mySeriesListModel.append(serie);
        }

        // must remember to call signal to let QML side know about populated items..
        emit seriesListChanged();
    }
}

void SeriesListModel::populateBannerList() {

    mySeriesListModel.clear();

    QList<QMap<QString, QString> > allSeries = mydbmanager->getSeries();
    int length = allSeries.size();
    for(int i = 0; i < length; ++i) {
        QMap<QString, QString> temp = allSeries.at(i);
        QString id = temp["id"];
        QMap<QString,QString> nextEpisodeDetails = mydbmanager->getNextEpisodeDetails(id.toInt());
        temp.unite(nextEpisodeDetails);
        SeriesData* series = new SeriesData(this, temp);
        mySeriesListModel.append(series);
    }
    emit seriesListChanged();
}

void SeriesListModel::selectSeries(int index) {

    myInfo = mySeriesListModel.at(index);

    myPoster = myInfo->getPoster();
    emit posterChanged();

    myNextEpisodeName = myInfo->getNextEpisodeName();
    emit nextEpisodeNameChanged();

    myNextEpisodeNumber = myInfo->getNextEpisodeNumber();
    emit nextEpisodeNumberChanged();

    myNextEpisodeSeasonNumber = myInfo->getNextEpisodeSeasonNumber();
    emit nextEpisodeSeasonNumberChanged();

    myDaysToNextEpisode = myInfo->getDaysToNextEpisode();
    emit daysToNextEpisodeChanged();

}

void SeriesListModel::storeSeries() {

    mySeries = myReader->getSeries();

    if(!mySeries.isEmpty()) {
        mydbmanager->insertSeries(mySeries.first());
    }
}

void SeriesListModel::storeEpisodes() {

    myEpisodes = myReader->getEpisodes();
    mydbmanager->insertEpisodes(myEpisodes);

}

void SeriesListModel::storeBanners() {

    myBanners = myReader->getBanners();

    // we are saving info for this series
    int seriesId = myInfo->getID().toInt();

    mydbmanager->insertBanners(myBanners, seriesId);
}

QString SeriesListModel::getID() { return myInfo->getID(); }

QString SeriesListModel::getLanguage() { return myInfo->getLanguage(); }

QString SeriesListModel::getSeriesName() { return myInfo->getSeriesName(); }

QString SeriesListModel::getAliasNames() { return myInfo->getAliasNames(); }

QString SeriesListModel::getBanner() { return myInfo->getBanner(); }

QString SeriesListModel::getOverview() { return myInfo->getOverview(); }

QString SeriesListModel::getFirstAired() { return myInfo->getFirstAired(); }

QString SeriesListModel::getIMDB_ID() { return myInfo->getIMDB_ID(); }

QString SeriesListModel::getZap2it_ID() { return myInfo->getZap2it_ID(); }

QString SeriesListModel::getNetwork() { return myInfo->getNetwork(); }

QString SeriesListModel::getNextEpisodeName() { return myNextEpisodeName; }

QString SeriesListModel::getNextEpisodeNumber() { return myNextEpisodeNumber; }

QString SeriesListModel::getNextEpisodeSeasonNumber() { return myNextEpisodeSeasonNumber; }

QString SeriesListModel::getDaysToNextEpisode() { return myDaysToNextEpisode; }

QString SeriesListModel::getStatus() { return myInfo->getStatus(); }

QString SeriesListModel::getRating() { return myInfo->getRating(); }

bool SeriesListModel::getLoading() { return myLoading; }

void SeriesListModel::setLoading(bool) {

    if(myLoading) {
        myLoading = false;
    } else {
        myLoading = true;
    }
    emit loadingChanged();
}

QString SeriesListModel::getPoster() {
    return myPoster;
}

QString SeriesListModel::getMode() {
    return mode;
}

void SeriesListModel::setMode(QString newmode) {

    if(mode !=  newmode) {
        mode = newmode;
        emit modeChanged();
        qDebug() << "mode changed!";
    }
}

void SeriesListModel::deleteSeries(int seriesID) {

    setLoading(true);
    if(mydbmanager->deleteSeries(seriesID)) {
        populateBannerList();
        emit updateModels();
    }
    setLoading(false);
}

void SeriesListModel::updateSeries(QString seriesID) {

    setLoading(true);
    myReader->getFullSeriesRecord(seriesID, "update");

}




