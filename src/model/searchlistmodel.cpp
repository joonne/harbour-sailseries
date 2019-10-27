#include "searchlistmodel.h"

SearchListModel::SearchListModel(QObject *parent, DatabaseManager *dbmanager, Api *api) :
    QObject(parent),
    m_dbmanager(dbmanager),
    m_api(api),
    m_loading(false),
    m_added(false)
{
    connect(m_api,
            SIGNAL(readyToPopulateSeries(QList<QVariantMap>)),
            this,
            SLOT(searchFinished(QList<QVariantMap>)));

    connect(m_dbmanager,
            SIGNAL(seriesStored()),
            this,
            SLOT(seriesStored()));
}

SearchListModel::~SearchListModel()
{
    for (auto series : m_searchListModel)
    {
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

void SearchListModel::searchFinished(QList<QVariantMap> series)
{
    populateSearchModel(series);
}

void SearchListModel::populateSearchModel(QList<QVariantMap> foundSeries)
{
    if (!foundSeries.empty())
    {
        for (auto series : foundSeries)
        {
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
    emit updateModels();
    setAdded(true);
}

void SearchListModel::searchSeries(QString text)
{
    setLoading(true);
    m_searchListModel.clear();
    emit searchModelChanged();
    m_api->searchSeries(text);
}

void SearchListModel::selectSeries(int index)
{
    m_info = m_searchListModel.at(index);
}

void SearchListModel::getAll(const int &id)
{
    m_api->getAll(id);
    setLoading(true);
}

int SearchListModel::getID() { return m_info->getID(); }

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
    if (m_added == cond)
    {
        return;
    }

    m_added = cond;
    emit addedChanged();
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
