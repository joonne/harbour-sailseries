#include "searchlistmodel.h"

SearchListModel::SearchListModel(QObject *parent, DatabaseManager *dbmanager, Api *api) :
    QObject(parent)
{
    m_dbmanager = dbmanager;
    m_api = api;

    connect(m_api,
            SIGNAL(readyToPopulateSeries(QList<QVariantMap>)),
            this,
            SLOT(searchFinished(QList<QVariantMap>)));

    connect(m_api,
            SIGNAL(readyToStoreSeries(QList<QVariantMap>, QList<QVariantMap>, QList<QVariantMap>)),
            this,
            SLOT(getAllFinished(QList<QVariantMap>, QList<QVariantMap>, QList<QVariantMap>)));

    connect(m_reader,
            SIGNAL(readyToPopulateSeries(MapOfMapLists)),
            this,
            SLOT(populateSearchModel(MapOfMapLists)));

    connect(m_reader,
            SIGNAL(readyToStoreSeries(MapOfMapLists)),
            this,
            SLOT(getFullSeriesRecordFinished(MapOfMapLists)));

    connect(this,
            SIGNAL(storeSeriesRequested(MapOfMapLists)),
            m_dbmanager,
            SLOT(storeSeries(MapOfMapLists)));

    connect(m_dbmanager,
            SIGNAL(seriesStored()),
            this,
            SLOT(seriesStored()));

    m_loading = false;
    m_added = false;
}

SearchListModel::~SearchListModel()
{
    for (auto series : m_searchListModel) {
        delete series;
        series = 0;
    }
}

QQmlListProperty<SeriesData> SearchListModel::getSearchModel()
{
    return QQmlListProperty<SeriesData>(this, &m_searchListModel, &SearchListModel::searchListCount, &SearchListModel::searchListAt);
}

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

<<<<<<< HEAD
void SearchListModel::searchFinished(QList<QVariantMap> series)
{
    populateSearchModel(series);
}

void SearchListModel::getAllFinished(QList<QVariantMap> series, QList<QVariantMap> episodes, QList<QVariantMap> banners)
{
    storeSeries(series);
    storeEpisodes(episodes);
    storeBanners(banners);
    setLoading(false);
    emit updateModels();
}

void SearchListModel::populateSearchModel(QList<QVariantMap> foundSeries)
{
    if (!foundSeries.empty()) {
        for (auto series : foundSeries) {
=======
void SearchListModel::getFullSeriesRecordFinished(QMap<QString, QList<QMap<QString, QString> > > seriesData)
{
    emit storeSeriesRequested(seriesData);
}

void SearchListModel::populateSearchModel(QMap<QString, QList<QMap<QString, QString> > > allSeries)
{
    m_series = allSeries["series"];
    if (!m_series.empty()) {
        for (auto series : m_series) {
>>>>>>> searchmodel threaded
            SeriesData* seriesData = new SeriesData(this, series);
            m_searchListModel.append(seriesData);
        }
        emit searchModelChanged();
        setLoading(false);
    }
}

void SearchListModel::seriesStored()
{
    setLoading(false);
//    emit updateModels();
    setAdded(true);
}

void SearchListModel::searchSeries(QString text)
{
    setLoading(true);
    m_searchListModel.clear();
    m_series.clear();
    emit searchModelChanged();
    m_api->searchSeries(text);
}

void SearchListModel::selectSeries(int index)
{
    m_info = m_searchListModel.at(index);
}

void SearchListModel::getFullSeriesRecord(QString id)
{
    m_api->getAll(id, "full");
    setLoading(true);
}

<<<<<<< HEAD
void SearchListModel::storeSeries(QList<QVariantMap> series)
{
    if (!series.isEmpty()) {
        m_dbmanager->insertSeries(series.first());
    }
}

void SearchListModel::storeEpisodes(QList<QVariantMap> episodes)
{
    auto seriesId = m_info->getID().toInt();
    m_dbmanager->insertEpisodes(episodes, seriesId);
    setAdded(true);
}

void SearchListModel::storeBanners(QList<QVariantMap> banners)
{
    auto seriesId = m_info->getID().toInt();
    m_dbmanager->insertBanners(banners, seriesId);
}

=======
>>>>>>> searchmodel threaded
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

void SearchListModel::setLoading(bool state)
{
    m_loading = state;
    emit loadingChanged();
}

bool SearchListModel::getAdded() { return m_added; }

void SearchListModel::setAdded(bool cond)
{
    if (m_added != cond) {
        m_added = cond;
        emit addedChanged();
    }
}

void SearchListModel::clearList()
{
    m_searchListModel.clear();
    emit searchModelChanged();
}

void SearchListModel::checkIfAdded(QString id, QString name)
{
    setAdded(m_dbmanager->isAlreadyAdded(id.toInt(), name));
}
