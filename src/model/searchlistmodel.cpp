#include "searchlistmodel.h"

SearchListModel::SearchListModel(QObject *parent, DatabaseManager *dbmanager, XMLReader *xmlreader) :
    QObject(parent)
{
    mydbmanager = dbmanager;
    myReader = xmlreader;

    connect(myReader,
            SIGNAL(readyToPopulateSeries()),
            this,
            SLOT(xmlParseFinished()));

    connect(myReader,
            SIGNAL(readyToStoreSeries()),
            this,
            SLOT(getFullSeriesRecordFinished()));

    myLoading = false;
    myAdded = false;
}

SearchListModel::~SearchListModel() {

    foreach(SeriesData* series, mySearchListModel) {
        delete series;
        series = 0;
    }

    qDebug() << "destructing SearchListModel";

}

/**
 * @brief returns listmodel
 * @return listmodel for the search page
 */
QQmlListProperty<SeriesData> SearchListModel::getSearchModel() {

    return QQmlListProperty<SeriesData>(this,&mySearchListModel,&SearchListModel::searchListCount,&SearchListModel::searchListAt);

}

// List handling methods

void SearchListModel::searchListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val)
{
    SearchListModel* searchModel = qobject_cast<SearchListModel*>(prop->object);
    searchModel->mySearchListModel.append(val);
}

SeriesData* SearchListModel::searchListAt(QQmlListProperty<SeriesData>* prop, int index)
{
    return (qobject_cast<SearchListModel*>(prop->object))->mySearchListModel.at(index);
}

int SearchListModel::searchListCount(QQmlListProperty<SeriesData>* prop)
{
    return qobject_cast<SearchListModel*>(prop->object)->mySearchListModel.size();
}

void SearchListModel::searchListClear(QQmlListProperty<SeriesData>* prop)
{
    qobject_cast<SearchListModel*>(prop->object)->mySearchListModel.clear();
}

void SearchListModel::xmlParseFinished() {

    mySeries = myReader->getSeries();
    populateSearchModel();
}

void SearchListModel::getFullSeriesRecordFinished() {

    storeSeries();
    storeEpisodes();
    storeBanners();
    setLoading(false);
    emit updateModels();
}

// -------------------------------------------------------------------
// POPULATING DATA

void SearchListModel::populateSearchModel() {


    if(mySeries.size() != 0) {
        int length = mySeries.size();
        for(int i = 0; i < length; ++i) {
            QMap<QString,QString> temp = mySeries.at(i);
            SeriesData* series = new SeriesData(this, temp);
            mySearchListModel.append(series);
        }

        // must remember to call signal to let QML side know about populated items..
        emit searchModelChanged();
        setLoading(false);
    }
}

/**
 * @brief search for series
 * @param text - series name to be searched
 */
void SearchListModel::searchSeries(QString text) {

    setLoading(true);
    mySearchListModel.clear();
    mySeries.clear();
    emit searchModelChanged();
    myReader->searchSeries(text);
}

/**
 * @brief selects series to be shown in the seriesinfopage
 * @param index - index of the listmodel
 */
void SearchListModel::selectSeries(int index) {

    myInfo = mySearchListModel.at(index);

}

/**
 * @brief get full series record from thetvdb
 * @param id - id of the series to retrieve
 */
void SearchListModel::getFullSeriesRecord(QString id) {
    myReader->getFullSeriesRecord(id,"full");
    setLoading(true);
}

/**
 * @brief store series to db
 */
void SearchListModel::storeSeries() {

    mySeries = myReader->getSeries();

    if(!mySeries.isEmpty()) {
        mydbmanager->insertSeries(mySeries.first());
    }
}

void SearchListModel::storeEpisodes() {

    myEpisodes = myReader->getEpisodes();

    mydbmanager->insertEpisodes(myEpisodes);

    setAdded(true);
}

void SearchListModel::storeBanners() {

    myBanners = myReader->getBanners();

    // we are saving info for this series
    int seriesId = myInfo->getID().toInt();

    mydbmanager->insertBanners(myBanners, seriesId);

}

QString SearchListModel::getID() { return myInfo->getID(); }

QString SearchListModel::getLanguage() { return myInfo->getLanguage(); }

QString SearchListModel::getSeriesName() { return myInfo->getSeriesName(); }

QString SearchListModel::getAliasNames() { return myInfo->getAliasNames(); }

QString SearchListModel::getBanner() { return myInfo->getBanner(); }

QString SearchListModel::getOverview() { return myInfo->getOverview(); }

QString SearchListModel::getFirstAired() { return myInfo->getFirstAired(); }

QString SearchListModel::getIMDB_ID() { return myInfo->getIMDB_ID(); }

QString SearchListModel::getZap2it_ID() { return myInfo->getZap2it_ID(); }

QString SearchListModel::getNetwork() { return myInfo->getNetwork(); }

bool SearchListModel::getLoading() { return myLoading; }

void SearchListModel::setLoading(bool) {

    if(myLoading) {
        myLoading = false;
    } else {
        myLoading = true;
    }
    emit loadingChanged();
}

bool SearchListModel::getAdded() { return myAdded; }

void SearchListModel::setAdded(bool cond) {

    if(myAdded != cond) {
        myAdded = cond;
        emit addedChanged();
    }

}

void SearchListModel::clearList() {

    mySearchListModel.clear();
    emit searchModelChanged();
}

void SearchListModel::checkIfAdded(QString id, QString name) {

    if(mydbmanager->isAlreadyAdded(id.toInt(), name)) {
        setAdded(true);
        qDebug() << "already added";
    } else {
        setAdded(false);
        qDebug() << "not added";
    }
}


