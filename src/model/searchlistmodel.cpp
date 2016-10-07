#include "searchlistmodel.h"

SearchListModel::SearchListModel(QObject *parent, DatabaseManager *dbmanager, XMLReader *xmlreader) :
    QObject(parent)
{
    m_dbmanager = dbmanager;
    m_reader = xmlreader;

    connect(m_reader,
            SIGNAL(readyToPopulateSeries()),
            this,
            SLOT(xmlParseFinished()));

    connect(m_reader,
            SIGNAL(readyToStoreSeries()),
            this,
            SLOT(getFullSeriesRecordFinished()));

    m_loading = false;
    m_added = false;
}

SearchListModel::~SearchListModel() {

    foreach(SeriesData* series, m_searchListModel) {
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

    return QQmlListProperty<SeriesData>(this,&m_searchListModel,&SearchListModel::searchListCount,&SearchListModel::searchListAt);

}

// List handling methods

void SearchListModel::searchListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val)
{
    SearchListModel* searchModel = qobject_cast<SearchListModel*>(prop->object);
    searchModel->m_searchListModel.append(val);
}

SeriesData* SearchListModel::searchListAt(QQmlListProperty<SeriesData>* prop, int index)
{
    return (qobject_cast<SearchListModel*>(prop->object))->m_searchListModel.at(index);
}

int SearchListModel::searchListCount(QQmlListProperty<SeriesData>* prop)
{
    return qobject_cast<SearchListModel*>(prop->object)->m_searchListModel.size();
}

void SearchListModel::searchListClear(QQmlListProperty<SeriesData>* prop)
{
    qobject_cast<SearchListModel*>(prop->object)->m_searchListModel.clear();
}

void SearchListModel::xmlParseFinished() {

    m_series = m_reader->getSeries();
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


    if (m_series.size() != 0) {
        int length = m_series.size();
        for(int i = 0; i < length; ++i) {
            QMap<QString,QString> temp = m_series.at(i);
            SeriesData* series = new SeriesData(this, temp);
            m_searchListModel.append(series);
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
    m_searchListModel.clear();
    m_series.clear();
    emit searchModelChanged();
    m_reader->searchSeries(text);
}

/**
 * @brief selects series to be shown in the seriesinfopage
 * @param index - index of the listmodel
 */
void SearchListModel::selectSeries(int index) {

    m_info = m_searchListModel.at(index);

}

/**
 * @brief get full series record from thetvdb
 * @param id - id of the series to retrieve
 */
void SearchListModel::getFullSeriesRecord(QString id) {
    m_reader->getFullSeriesRecord(id,"full");
    setLoading(true);
}

/**
 * @brief store series to db
 */
void SearchListModel::storeSeries() {

    m_series = m_reader->getSeries();

    if (!m_series.isEmpty()) {
        m_dbmanager->insertSeries(m_series.first());
    }
}

void SearchListModel::storeEpisodes() {

    m_episodes = m_reader->getEpisodes();

    m_dbmanager->insertEpisodes(m_episodes);

    setAdded(true);
}

void SearchListModel::storeBanners() {

    m_banners = m_reader->getBanners();

    // we are saving info for this series
    int seriesId = m_info->getID().toInt();

    m_dbmanager->insertBanners(m_banners, seriesId);

}

QString SearchListModel::getID() { return m_info->getID(); }

QString SearchListModel::getLanguage() { return m_info->getLanguage(); }

QString SearchListModel::getSeriesName() { return m_info->getSeriesName(); }

QString SearchListModel::getAliasNames() { return m_info->getAliasNames(); }

QString SearchListModel::getBanner() { return m_info->getBanner(); }

QString SearchListModel::getOverview() { return m_info->getOverview(); }

QString SearchListModel::getFirstAired() { return m_info->getFirstAired(); }

QString SearchListModel::getIMDB_ID() { return m_info->getIMDB_ID(); }

QString SearchListModel::getZap2it_ID() { return m_info->getZap2it_ID(); }

QString SearchListModel::getNetwork() { return m_info->getNetwork(); }

bool SearchListModel::getLoading() { return m_loading; }

void SearchListModel::setLoading(bool) {

    if (m_loading) {
        m_loading = false;
    } else {
        m_loading = true;
    }
    emit loadingChanged();
}

bool SearchListModel::getAdded() { return m_added; }

void SearchListModel::setAdded(bool cond) {

    if (m_added != cond) {
        m_added = cond;
        emit addedChanged();
    }

}

void SearchListModel::clearList() {

    m_searchListModel.clear();
    emit searchModelChanged();
}

void SearchListModel::checkIfAdded(QString id, QString name) {
    setAdded(m_dbmanager->isAlreadyAdded(id.toInt(), name));
}


