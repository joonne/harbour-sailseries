#include "engine.h"

Engine::Engine(QObject *parent) :
    QObject(parent),
    m_api(new Api),
    m_dbmanager(new DatabaseManager),
    m_loading(false)
{
    auto dbThread = new QThread;
    m_dbmanager->moveToThread(dbThread);
    connect(dbThread, SIGNAL(finished()), dbThread, SLOT(deleteLater()));
    dbThread->start();

    m_seriesListModel = new SeriesListModel(this, m_dbmanager);
    m_searchListModel = new SearchListModel(this);
    m_todayListModel = new TodayListModel(this, m_dbmanager, m_api);
    m_episodeListModel = new EpisodeListModel(this);
    m_seasonListModel = new SeasonListModel(this);
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
            SIGNAL(storePosterImageFor(int,QString)),
            m_dbmanager,
            SLOT(storePosterImageFor(int,QString)));

    connect(m_api,
            SIGNAL(storeBannerImageFor(int,QString)),
            m_dbmanager,
            SLOT(storeBannerImageFor(int,QString)));

    connect(m_api,
            SIGNAL(storeFanartImageFor(int,QString)),
            m_dbmanager,
            SLOT(storeFanartImageFor(int,QString)));

    connect(m_api,
            SIGNAL(storeEpisodes(int, QList<QVariantMap>)),
            m_dbmanager,
            SLOT(storeEpisodes(int, QList<QVariantMap>)));

    connect(m_api,
            SIGNAL(storeSeries(QVariantMap)),
            m_dbmanager,
            SLOT(storeSeries(QVariantMap)));

    connect(m_episodeListModel,
            SIGNAL(getEpisodesRequested(int,int)),
            m_dbmanager,
            SLOT(getEpisodes(int,int)));

    connect(m_dbmanager,
            SIGNAL(populateEpisodeList(QList<QVariantMap>)),
            m_episodeListModel,
            SLOT(populateEpisodeList(QList<QVariantMap>)));

    connect(m_episodeListModel,
            SIGNAL(toggleWatchedRequested(int,int,int)),
            m_dbmanager,
            SLOT(toggleWatched(int,int,int)));

    connect(m_episodeListModel,
            SIGNAL(markSeasonAsWatchedRequested(int,int)),
            m_dbmanager,
            SLOT(markSeasonAsWatched(int,int)));

    connect(m_seasonListModel,
            SIGNAL(getSeasonsRequested(int)),
            m_dbmanager,
            SLOT(getSeasons(int)));

    connect(m_dbmanager,
            SIGNAL(populateSeasonList(QList<QVariantMap>)),
            m_seasonListModel,
            SLOT(populateSeasonList(QList<QVariantMap>)));

    connect(m_searchListModel,
            SIGNAL(searchSeriesRequested(QString)),
            m_api,
            SLOT(searchSeries(QString)));

    connect(m_searchListModel,
            SIGNAL(getAllRequested(int)),
            m_api,
            SLOT(getAll(int)));

    connect(m_seriesListModel,
            SIGNAL(getAllRequested(int)),
            m_api,
            SLOT(getAll(int)));

    connect(this,
            SIGNAL(getEpisode(int)),
            m_api,
            SLOT(getEpisode(int)));

    connect(m_api,
            SIGNAL(readyToPopulateSeries(QList<QVariantMap>)),
            m_searchListModel,
            SLOT(searchFinished(QList<QVariantMap>)));

    connect(m_dbmanager,
            SIGNAL(seriesStored()),
            m_searchListModel,
            SLOT(seriesStored()));

    connect(m_searchListModel,
            SIGNAL(checkIfAddedRequested(int,QString)),
            m_dbmanager,
            SLOT(checkIfAdded(int,QString)));

    connect(m_dbmanager,
            SIGNAL(checkIfAddedReady(bool)),
            m_searchListModel,
            SLOT(checkIfAddedReady(bool)));
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
    emit m_statistics->requestStatistics();
    emit m_seasonListModel->getSeasons(m_seriesListModel->getID());
}

void Engine::updateModels()
{
    emit m_todayListModel->getStartPageSeries();
    emit m_seriesListModel->getSeries();
    emit m_statistics->requestStatistics();
    emit m_seasonListModel->getSeasons(m_seriesListModel->getID());
}

void Engine::readyToUpdateEpisodeDetails(const QVariantMap &episode)
{
    emit updateEpisodeDetails(episode);
}

void Engine::toggleLoading(bool state) { m_loading = state; }

bool Engine::deleteDuplicateEpisodes()
{
    return m_dbmanager->deleteDuplicateEpisodes();
}

void Engine::requestEpisodeDetails(const int &id)
{
    emit getEpisode(id);
}
