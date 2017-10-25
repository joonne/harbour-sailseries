#include "seasonlistmodel.h"

SeasonListModel::SeasonListModel(QObject *parent,  DatabaseManager* dbmanager) :
    QObject(parent)
{
    m_dbmanager = dbmanager;
}

SeasonListModel::~SeasonListModel()
{
    foreach (auto season, m_seasonListModel) {
        delete season;
        season = 0;
    }
    qDebug() << "destructing SeasonListModel";
}

QQmlListProperty<SeasonData> SeasonListModel::getSeasonList()
{
    return QQmlListProperty<SeasonData>(this, &m_seasonListModel, &SeasonListModel::seasonListCount, &SeasonListModel::seasonListAt);
}

// list handling methods

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

void SeasonListModel::populateSeasonList(QString seriesID)
{
    m_seasonListModel.clear();

    int seasonsCount = m_dbmanager->seasonCount(seriesID.toInt());

    qDebug() << "seasonCount" << seasonsCount;

    for (int i = 1; i <= seasonsCount; ++i) {

        QString banner = m_dbmanager->getSeasonBanner(seriesID.toInt(),i);
        int watchedCount = m_dbmanager->watchedCountBySeason(seriesID.toInt(),i);
        int totalCount = m_dbmanager->totalCountBySeason(seriesID.toInt(),i);

        SeasonData* seasonData = new SeasonData(this, i, banner, watchedCount, totalCount);
        m_seasonListModel.append(seasonData);

    }
    emit seasonListChanged();
}
