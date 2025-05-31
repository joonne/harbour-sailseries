#include "searchlistmodel.h"

SearchListModel::SearchListModel(QObject *parent, Api* api, DatabaseManager *dbmanager) :
    QObject(parent),
    m_api(api),
    m_dbmanager(dbmanager)
{
    connect(this,
            SIGNAL(searchSeriesRequested(QString)),
            m_api,
            SLOT(searchSeries(QString)));

    connect(this,
            SIGNAL(getAllRequested(int)),
            m_api,
            SLOT(getAll(int)));

    connect(m_api,
            SIGNAL(readyToPopulateSeries(QList<QVariantMap>)),
            this,
            SLOT(searchFinished(QList<QVariantMap>)));

    connect(m_dbmanager,
            SIGNAL(seriesStored(int)),
            this,
            SLOT(seriesStored(int)));
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
    connect(m_dbmanager, &DatabaseManager::getSeriesNamesReady, [this, foundSeries](QSet<QString> seriesNames)
    {
        m_searchListModel.clear();
        emit searchModelChanged();

        for (auto series : foundSeries)
        {
            series["isAdded"] = seriesNames.contains(series["seriesName"].toString());
            auto seriesData = new SeriesData(0, series);
            m_searchListModel.append(seriesData);
        }

        emit searchModelChanged();
    });

    emit getSeriesNames();
}

void SearchListModel::seriesStored(const int &seriesId)
{
    Q_UNUSED(seriesId);
    emit setLoading(false);
    emit updateModels();
}

void SearchListModel::searchSeries(const QString &text)
{
    emit setLoading(true);
    emit searchSeriesRequested(text);
}

void SearchListModel::getAll(const int &seriesId)
{
    emit getAllRequested(seriesId);
    emit setLoading(true);
}

void SearchListModel::clearList()
{
    m_searchListModel.clear();
    emit searchModelChanged();
}
