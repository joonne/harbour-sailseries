#include "engine.h"

Engine::Engine(QObject *parent) :
    QObject(parent),
    m_reader(new XMLReader),
    m_dbmanager(new DatabaseManager),
    m_loading(false)
{
    typedef QList<QVariantMap> VariantList;
    qRegisterMetaType<VariantList>("VariantList");

    typedef QList<QMap<QString, QString> > MapList;
    qRegisterMetaType<MapList>("MapList");

    typedef QMap<QString, MapList > MapOfMapLists;
    qRegisterMetaType<MapOfMapLists>("MapOfMapLists");

    QThread* db_thread = new QThread;
    m_dbmanager->moveToThread(db_thread);
    connect(db_thread, SIGNAL(finished()), db_thread, SLOT(deleteLater()));
    db_thread->start();

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
