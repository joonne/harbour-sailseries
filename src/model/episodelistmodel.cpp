#include "episodelistmodel.h"

EpisodeListModel::EpisodeListModel(QObject *parent, DatabaseManager *dbmanager) :
    QObject(parent), m_dbmanager(dbmanager)
{
    connect(this,
            SIGNAL(getEpisodesRequested(int,int)),
            m_dbmanager,
            SLOT(getEpisodes(int,int)));

    connect(m_dbmanager,
            SIGNAL(populateEpisodeList(MapList)),
            this,
            SLOT(populateEpisodeList(MapList)));

    connect(this,
            SIGNAL(toggleWatchedRequested(QString, QString, int)),
            m_dbmanager,
            SLOT(toggleWatched(QString, QString, int)));
}

EpisodeListModel::~EpisodeListModel()
{
    for (auto episode : m_episodeListModel) {
        delete episode;
        episode = 0;
    }
}

QQmlListProperty<EpisodeData> EpisodeListModel::getEpisodeList()
{
    return QQmlListProperty<EpisodeData>(this, &m_episodeListModel, &EpisodeListModel::episodeListCount, &EpisodeListModel::episodeListAt);
}

void EpisodeListModel::episodeListAppend(QQmlListProperty<EpisodeData>* prop, EpisodeData* val)
{
    EpisodeListModel* episodeModel = qobject_cast<EpisodeListModel*>(prop->object);
    episodeModel->m_episodeListModel.append(val);
}

EpisodeData* EpisodeListModel::episodeListAt(QQmlListProperty<EpisodeData>* prop, int index)
{
    return (qobject_cast<EpisodeListModel*>(prop->object))->m_episodeListModel.at(index);
}

int EpisodeListModel::episodeListCount(QQmlListProperty<EpisodeData>* prop)
{
    return qobject_cast<EpisodeListModel*>(prop->object)->m_episodeListModel.size();
}

void EpisodeListModel::episodeListClear(QQmlListProperty<EpisodeData>* prop)
{
    qobject_cast<EpisodeListModel*>(prop->object)->m_episodeListModel.clear();
}

void EpisodeListModel::getEpisodes(QString seriesId, int seasonNumber)
{
    emit getEpisodesRequested(seriesId.toInt(), seasonNumber);
}

void EpisodeListModel::populateEpisodeList(QList<QMap<QString, QString> > episodes)
{
    m_episodeListModel.clear();
    emit episodeListChanged();

    if (episodes.size() != 0) {
        for (auto episode : episodes) {
            auto episodeData = new EpisodeData(this, episode);
            m_episodeListModel.append(episodeData);
        }

        emit episodeListChanged();
    }
}

void EpisodeListModel::toggleWatched(QString episodeId, QString seriesId, int seasonNumber)
{
    emit toggleWatchedRequested(episodeId, seriesId, seasonNumber);
}

void EpisodeListModel::markSeasonWatched(QString seriesId, int season)
{
    m_dbmanager->markSeasonWatched(seriesId.toInt(), season);
    emit getEpisodesRequested(seriesId.toInt(), season);
}

int EpisodeListModel::getSeasonCount(QString seriesId)
{
    return m_dbmanager->seasonCount(seriesId.toInt());
}
