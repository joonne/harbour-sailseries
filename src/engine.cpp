#include "engine.h"

Engine::Engine(QObject *parent) :
    QObject(parent),
    m_reader(new XMLReader),
    m_dbmanager(new DatabaseManager)
{
    m_dbmanager->setUpDB();

    m_seriesListModel = new SeriesListModel(this, m_dbmanager, m_reader);
    m_searchListModel = new SearchListModel(this, m_dbmanager, m_reader);
    m_todayListModel = new TodayListModel(this, m_dbmanager, m_reader);
    m_episodeListModel = new EpisodeListModel(this, m_dbmanager);
    m_seasonListModel = new SeasonListModel(this, m_dbmanager);

    connect(m_searchListModel,
            SIGNAL(updateModels()),
            this,
            SLOT(readyToUpdateModels()));

    connect(m_seriesListModel,
            SIGNAL(updateModels()),
            this,
            SLOT(readyToUpdateModels()));

}

Engine::~Engine() {

    delete m_reader;
    delete m_dbmanager;

    qDebug() << "destructing Engine";

}

SeriesListModel* Engine::getSeriesListModel() { return m_seriesListModel; }

SearchListModel *Engine::getSearchModel() { return m_searchListModel; }

TodayListModel* Engine::getTodayModel() { return m_todayListModel; }

EpisodeListModel* Engine::getEpisodeListModel() { return m_episodeListModel; }

SeasonListModel* Engine::getSeasonListModel() { return m_seasonListModel; }

void Engine::readyToUpdateModels() {
    m_todayListModel->populateTodayModel();
    m_seriesListModel->populateBannerList();
}

void Engine::updateModels() {
    qDebug() << "update";
    m_todayListModel->populateTodayModel();
    m_seriesListModel->populateBannerList();
}
