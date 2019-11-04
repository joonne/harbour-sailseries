#include "seasonlistmodel.h"

SeasonListModel::SeasonListModel(QObject *parent, DatabaseManager *dbmanager) :
    QObject(parent),
    m_dbmanager(dbmanager)
{
    connect(this,
            SIGNAL(getSeasonsRequested(int)),
            m_dbmanager,
            SLOT(getSeasons(int)));

    connect(m_dbmanager,
            SIGNAL(populateSeasonList(QList<QVariantMap>)),
            this,
            SLOT(populateSeasonList(QList<QVariantMap>)));
}

SeasonListModel::~SeasonListModel()
{
    for (auto season : m_seasonListModel)
    {
        delete season;
        season = 0;
    }
}

QQmlListProperty<SeasonData> SeasonListModel::getSeasonList()
{
    return QQmlListProperty<SeasonData>(this, &m_seasonListModel, &SeasonListModel::seasonListCount, &SeasonListModel::seasonListAt);
}

void SeasonListModel::seasonListAppend(QQmlListProperty<SeasonData>* prop, SeasonData* val)
{
    SeasonListModel* seasonListModel = qobject_cast<SeasonListModel*>(prop->object);
    seasonListModel->m_seasonListModel.append(val);
}

SeasonData* SeasonListModel::seasonListAt(QQmlListProperty<SeasonData>* prop, int index)
{
    return (qobject_cast<SeasonListModel*>(prop->object))->m_seasonListModel.at(index);
}

int SeasonListModel::seasonListCount(QQmlListProperty<SeasonData>* prop)
{
    return qobject_cast<SeasonListModel*>(prop->object)->m_seasonListModel.size();
}

void SeasonListModel::seasonListClear(QQmlListProperty<SeasonData>* prop)
{
    qobject_cast<SeasonListModel*>(prop->object)->m_seasonListModel.clear();
}

void SeasonListModel::getSeasons(const int &seriesId)
{
    emit getSeasonsRequested(seriesId);
}

void SeasonListModel::populateSeasonList(const QList<QVariantMap> &seasons)
{
    m_seasonListModel.clear();

    const auto length = seasons.size();
    for (auto i = 0; i < length; ++i)
    {
        auto season = seasons.at(i);
        auto banner = season["banner"].toString();
        auto watchedCount = season["watchedCount"].toInt();
        auto totalCount = season["totalCount"].toInt();

        auto seasonData = new SeasonData(this, i, banner, watchedCount, totalCount);
        m_seasonListModel.append(seasonData);
    }

    emit seasonListChanged();
}
