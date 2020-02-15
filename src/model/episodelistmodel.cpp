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

void EpisodeListModel::getEpisodes(const int &seriesId, const int &seasonNumber)
{
    emit getEpisodesRequested(seriesId, seasonNumber);
}

void EpisodeListModel::populateEpisodeList(const QList<QVariantMap> &episodes)
{
    m_listModel.clear();
    emit episodeListChanged();

    for (auto episode : episodes)
    {
        auto episodeData = new EpisodeData(this, episode);
        m_listModel.append(episodeData);
    }

    emit episodeListChanged();
}

void EpisodeListModel::setWatchedInModel(const int &episodeId, const bool &watched)
{
    for (auto episode: m_listModel)
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
