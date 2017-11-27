#include "engine.h"

Engine::Engine(QObject *parent) : QObject(parent), m_api(new Api), m_dbmanager(new DatabaseManager), m_loading(false)
{
    m_dbmanager->setUpDB();

    m_seriesListModel = new SeriesListModel(this, m_dbmanager, m_api);
    m_searchListModel = new SearchListModel(this, m_dbmanager, m_api);
    m_todayListModel = new TodayListModel(this, m_dbmanager, m_api);
    m_episodeListModel = new EpisodeListModel(this, m_dbmanager);
    m_seasonListModel = new SeasonListModel(this, m_dbmanager);
    m_statistics = new Statistics(this, m_dbmanager);

    connect(m_searchListModel,
            SIGNAL(updateModels()),
            this,
            SLOT(readyToUpdateModels()));

    connect(m_seriesListModel,
            SIGNAL(updateModels()),
            this,
            SLOT(readyToUpdateModels()));

    connect(m_api,
            SIGNAL(readyToPopulateEpisodeDetails(QVariantMap)),
            this,
            SLOT(readyToUpdateEpisodeDetails(QVariantMap)));
}

Engine::~Engine()
{
    delete m_api;
    delete m_dbmanager;

    qDebug() << "destructing Engine";
}

SeriesListModel* Engine::getSeriesListModel() { return m_seriesListModel; }

SearchListModel *Engine::getSearchModel() { return m_searchListModel; }

TodayListModel* Engine::getTodayModel() { return m_todayListModel; }

EpisodeListModel* Engine::getEpisodeListModel() { return m_episodeListModel; }

SeasonListModel* Engine::getSeasonListModel() { return m_seasonListModel; }

Statistics* Engine::getStatistics() { return m_statistics; }

bool Engine::getLoading() { return m_loading; }

void Engine::readyToUpdateModels()
{
    m_todayListModel->populateTodayModel();
    m_seriesListModel->populateBannerList();
}

void Engine::updateModels()
{
    toggleLoading(true);
    m_todayListModel->populateTodayModel();
    m_seriesListModel->populateBannerList();
    m_statistics->updateStatistics();
    toggleLoading(false);
}

void Engine::readyToUpdateEpisodeDetails(QVariantMap episode)
{
    qDebug() << episode;
    emit updateEpisodeDetails(episode);
}

void Engine::toggleLoading(bool state) { m_loading = state; }

bool Engine::deleteDuplicateEpisodes() { return m_dbmanager->deleteDuplicateEpisodes(); }

void Engine::requestEpisodeDetails(QString id) { m_api->getEpisode(id); }
