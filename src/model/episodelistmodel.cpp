#include "episodelistmodel.h"

EpisodeListModel::EpisodeListModel(QObject *parent, DatabaseManager *dbmanager) :
    QObject(parent),
    m_dbmanager(dbmanager)
{
    connect(this,
            SIGNAL(getEpisodesRequested(int,int)),
            m_dbmanager,
            SLOT(getEpisodes(int,int)));

    connect(m_dbmanager,
            SIGNAL(populateEpisodeList(QList<QVariantMap>)),
            this,
            SLOT(populateEpisodeList(QList<QVariantMap>)));

    connect(this,
            SIGNAL(setWatchedRequested(int,int,bool)),
            m_dbmanager,
            SLOT(setWatched(int,int,bool)));

    connect(this,
            SIGNAL(markSeasonAsWatchedRequested(int,int)),
            m_dbmanager,
            SLOT(markSeasonAsWatched(int,int)));

    connect(m_dbmanager,
            SIGNAL(setWatchedReady(int,bool)),
            this,
            SLOT(setWatchedInModel(int,bool)));
}

EpisodeListModel::~EpisodeListModel()
{
    for (auto episode : m_episodeListModel)
    {
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

void EpisodeListModel::getEpisodes(const int &seriesId, const int &seasonNumber)
{
    emit getEpisodesRequested(seriesId, seasonNumber);
}

void EpisodeListModel::populateEpisodeList(const QList<QVariantMap> &episodes)
{
    m_episodeListModel.clear();
    emit episodeListChanged();

    for (auto episode : episodes)
    {
        auto episodeData = new EpisodeData(this, episode);
        m_episodeListModel.append(episodeData);
    }

    emit episodeListChanged();
}

void EpisodeListModel::setWatchedInModel(const int &episodeId, const bool &watched)
{
    for (auto episode: m_episodeListModel)
    {
        if (episode->getID() == episodeId)
        {
            episode->setWatched(watched);
            break;
        }
    }
}

void EpisodeListModel::setWatched(const int &episodeId, const int &seriesId, const bool &watched)
{
    emit setWatchedRequested(episodeId, seriesId, watched);
}

void EpisodeListModel::markSeasonAsWatched(const int &seriesId, const int &season)
{
    emit markSeasonAsWatchedRequested(seriesId, season);
}
