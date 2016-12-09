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

SearchListModel::~SearchListModel()
{
    foreach(SeriesData* series, m_searchListModel) {
        delete series;
        series = 0;
    }
    qDebug() << "destructing SearchListModel";
}

QQmlListProperty<SeriesData> SearchListModel::getSearchModel()
{
    return QQmlListProperty<SeriesData>(this, &m_searchListModel, &SearchListModel::searchListCount, &SearchListModel::searchListAt);
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

void SearchListModel::xmlParseFinished()
{
    m_series = m_reader->getSeries();
    populateSearchModel();
}

void SearchListModel::getFullSeriesRecordFinished()
{
    storeSeries();
    storeEpisodes();
    storeBanners();
    setLoading(false);
    emit updateModels();
}

void SearchListModel::populateSearchModel()
{
    if (!m_series.empty()) {
        for (auto series : m_series) {
            SeriesData* seriesData = new SeriesData(this, series);
            m_searchListModel.append(seriesData);
        }
        emit searchModelChanged();
        setLoading(false);
    }
}

void SearchListModel::searchSeries(QString text)
{
    setLoading(true);
    m_searchListModel.clear();
    m_series.clear();
    emit searchModelChanged();
    m_reader->searchSeriesNew(text);
}

void SearchListModel::selectSeries(int index)
{
    m_info = m_searchListModel.at(index);
}

void SearchListModel::getFullSeriesRecord(QString id)
{
    m_reader->getFullSeriesRecord(id, "full");
    setLoading(true);
}

void SearchListModel::storeSeries()
{
    m_series = m_reader->getSeries();
    if (!m_series.isEmpty()) {
        m_dbmanager->insertSeries(m_series.first());
    }
}

void SearchListModel::storeEpisodes()
{
    m_episodes = m_reader->getEpisodes();
    m_dbmanager->insertEpisodes(m_episodes);
    setAdded(true);
}

void SearchListModel::storeBanners()
{
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
