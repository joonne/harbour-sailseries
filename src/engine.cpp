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

    m_seriesListModel = new SeriesListModel(this, m_api, m_dbmanager);
    m_searchListModel = new SearchListModel(this, m_api, m_dbmanager);
    m_todayListModel = new TodayListModel(this, m_dbmanager);
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

    connect(m_api,
            SIGNAL(storeSeasonImages(int,QList<QVariantMap>)),
            m_dbmanager,
            SLOT(storeSeasonImages(int,QList<QVariantMap>)));

    connect(this,
            SIGNAL(getEpisodeDetails(int)),
            m_api,
            SLOT(getEpisodeDetails(int)));

    connect(m_searchListModel,
            SIGNAL(setLoading(bool)),
            this,
            SLOT(setLoading(bool)));

    connect(m_seriesListModel,
            SIGNAL(setLoading(bool)),
            this,
            SLOT(setLoading(bool)));
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
    updateModels();
}

void Engine::updateModels()
{
    emit m_todayListModel->getStartPageSeries();
    emit m_seriesListModel->getSeries();
    emit m_statistics->requestStatistics();
    // emit m_seasonListModel->getSeasons(m_seriesListModel->getID());
}

void Engine::readyToUpdateEpisodeDetails(const QVariantMap &episode)
{
    emit updateEpisodeDetails(episode);
}

void Engine::setLoading(bool state)
{
    m_loading = state;
    emit loadingChanged();
}

void Engine::deleteDuplicateEpisodes()
{
    emit deleteDuplicateEpisodesRequested();
}

void Engine::requestEpisodeDetails(const int &id)
{
    emit getEpisodeDetails(id);
}
