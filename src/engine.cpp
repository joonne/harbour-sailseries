#include "engine.h"

Engine::Engine(QObject *parent) :
    QObject(parent),
    m_api(new Api),
    m_dbmanager(new DatabaseManager),
    m_loading(false)
{
    QThread* dbThread = new QThread;
    m_dbmanager->moveToThread(dbThread);
    connect(dbThread, SIGNAL(finished()), dbThread, SLOT(deleteLater()));
    dbThread->start();

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

    connect(m_api,
            SIGNAL(storePosterImageFor(QString,QString)),
            m_dbmanager,
            SLOT(storePosterImageFor(QString,QString)));

    connect(m_api,
            SIGNAL(storeBannerImageFor(QString,QString)),
            m_dbmanager,
            SLOT(storeBannerImageFor(QString,QString)));

    connect(m_api,
            SIGNAL(storeFanartImageFor(QString,QString)),
            m_dbmanager,
            SLOT(storeFanartImageFor(QString,QString)));

    connect(m_api,
            SIGNAL(storeEpisodes(QString, QList<QVariantMap>)),
            m_dbmanager,
            SLOT(storeEpisodes(QString, QList<QVariantMap>)));

    connect(m_api,
            SIGNAL(storeSeries(QVariantMap)),
            m_dbmanager,
            SLOT(storeSeries(QVariantMap)));
}

Engine::~Engine()
{
    delete m_api;
    m_api = 0;

    delete m_dbmanager;
    m_dbmanager = 0;
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
    emit m_todayListModel->getStartPageSeries();
    emit m_seriesListModel->getSeries();
}

void Engine::updateModels()
{
    emit m_todayListModel->getStartPageSeries();
    emit m_seriesListModel->getSeries();
    emit m_statistics->requestStatistics();
}

void Engine::readyToUpdateEpisodeDetails(QVariantMap episode)
{
    emit updateEpisodeDetails(episode);
}

void Engine::toggleLoading(bool state) { m_loading = state; }

bool Engine::deleteDuplicateEpisodes() { return m_dbmanager->deleteDuplicateEpisodes(); }

void Engine::requestEpisodeDetails(QString id) { m_api->getEpisode(id); }
