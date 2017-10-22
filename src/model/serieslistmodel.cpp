#include "serieslistmodel.h"

SeriesListModel::SeriesListModel(QObject *parent, DatabaseManager* dbmanager, Api *api) :
    QObject(parent)
{

    m_api = api;
    m_dbmanager = dbmanager;

    mode = "default";

    connect(m_api,
            SIGNAL(readyToUpdateSeries()),
            this,
            SLOT(updateFetchFinished()));

    populateBannerList();

    m_loading = false;
}

SeriesListModel::~SeriesListModel()
{
    foreach(auto series, m_seriesListModel) {
        delete series;
        series = 0;
    }
    qDebug() << "destructing SeriesListModel";
}

void SeriesListModel::updateFetchFinished()
{
    storeSeries();
    storeEpisodes();
    storeBanners();

    if (!m_seriesIds.isEmpty()) {
        updateSeries();
        return;
    }

    setLoading(false);
    populateBannerList();
}

QQmlListProperty<SeriesData> SeriesListModel::getSeriesList()
{
    return QQmlListProperty<SeriesData>(this, &m_seriesListModel, &SeriesListModel::seriesListCount, &SeriesListModel::seriesListAt);
}

void SeriesListModel::seriesListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val)
{
    SeriesListModel* seriesModel = qobject_cast<SeriesListModel*>(prop->object);
    seriesModel->m_seriesListModel.append(val);
}

SeriesData* SeriesListModel::seriesListAt(QQmlListProperty<SeriesData>* prop, int index)
{
    return (qobject_cast<SeriesListModel*>(prop->object))->m_seriesListModel.at(index);
}

int SeriesListModel::seriesListCount(QQmlListProperty<SeriesData>* prop)
{
    return qobject_cast<SeriesListModel*>(prop->object)->m_seriesListModel.size();
}

void SeriesListModel::seriesListClear(QQmlListProperty<SeriesData>* prop)
{
    qobject_cast<SeriesListModel*>(prop->object)->m_seriesListModel.clear();
}

void SeriesListModel::populateBannerList()
{
    m_seriesListModel.clear();
    emit seriesListChanged();

    auto allSeries = m_dbmanager->getSeries();
    for (auto series : allSeries) {
        auto id = series["id"];
        auto nextEpisodeDetails = m_dbmanager->getNextEpisodeDetails(id.toInt());
        series.unite(nextEpisodeDetails);
        auto seriesData = new SeriesData(this, series);
        m_seriesListModel.append(seriesData);
    }
    emit seriesListChanged();
}

void SeriesListModel::selectSeries(int index)
{
    m_info = m_seriesListModel.at(index);

    m_poster = m_info->getPoster();
    emit posterChanged();

    m_nextEpisodeName = m_info->getNextEpisodeName();
    emit nextEpisodeNameChanged();

    m_nextEpisodeNumber = m_info->getNextEpisodeNumber();
    emit nextEpisodeNumberChanged();

    m_nextEpisodeSeasonNumber = m_info->getNextEpisodeSeasonNumber();
    emit nextEpisodeSeasonNumberChanged();

    m_daysToNextEpisode = m_info->getDaysToNextEpisode();
    emit daysToNextEpisodeChanged();
}

void SeriesListModel::storeSeries()
{
    m_series = m_api->series();

    if (!m_series.isEmpty()) {
        m_dbmanager->insertSeries(m_series.first());
    }
}

void SeriesListModel::storeEpisodes()
{
    m_episodes = m_api->episodes();
    m_dbmanager->insertEpisodes(m_episodes);
}

void SeriesListModel::storeBanners()
{
    m_banners = m_api->banners();

    if (!m_series.isEmpty()) {
        int seriesId = m_series.first()["id"].toInt();
        m_dbmanager->insertBanners(m_banners, seriesId);
    }
}

QString SeriesListModel::getID() { return m_info->getID(); }

QString SeriesListModel::getLanguage() { return m_info->getLanguage(); }

QString SeriesListModel::getSeriesName() { return m_info->getSeriesName(); }

QString SeriesListModel::getAliasNames() { return m_info->getAliasNames(); }

QString SeriesListModel::getBanner() { return m_info->getBanner(); }

QString SeriesListModel::getOverview() { return m_info->getOverview(); }

QString SeriesListModel::getFirstAired() { return m_info->getFirstAired(); }

QString SeriesListModel::getIMDB_ID() { return m_info->getIMDB_ID(); }

QString SeriesListModel::getZap2it_ID() { return m_info->getZap2it_ID(); }

QString SeriesListModel::getNetwork() { return m_info->getNetwork(); }

QString SeriesListModel::getNextEpisodeName() { return m_nextEpisodeName; }

QString SeriesListModel::getNextEpisodeNumber() { return m_nextEpisodeNumber; }

QString SeriesListModel::getNextEpisodeSeasonNumber() { return m_nextEpisodeSeasonNumber; }

QString SeriesListModel::getDaysToNextEpisode() { return m_daysToNextEpisode; }

QString SeriesListModel::getStatus() { return m_info->getStatus(); }

QString SeriesListModel::getRating() { return m_info->getRating(); }

QString SeriesListModel::getGenre() { return m_info->getGenre(); }

bool SeriesListModel::getLoading() { return m_loading; }

void SeriesListModel::setLoading(bool state)
{
    if (m_loading != state) {
        m_loading = state;
        emit loadingChanged();
    }
}

QString SeriesListModel::getPoster() { return m_poster; }

QString SeriesListModel::getMode() { return mode; }

void SeriesListModel::setMode(QString newmode)
{
    if (mode != newmode) {
        mode = newmode;
        emit modeChanged();
    }
}

void SeriesListModel::deleteSeries(int seriesID)
{
    setLoading(true);
    if (m_dbmanager->deleteSeries(seriesID)) {
        populateBannerList();
        emit updateModels();
    }
    setLoading(false);
}

void SeriesListModel::updateSeries(QString seriesId)
{
    if (!m_seriesIds.isEmpty() && seriesId.isEmpty()) {
        seriesId = m_seriesIds.takeFirst();
    }

    setLoading(true);
    m_api->getSeries(seriesId, "update");

}

void SeriesListModel::updateAllSeries(bool updateEndedSeries)
{
    m_seriesIds.clear();
    m_series.clear();
    m_episodes.clear();
    m_banners.clear();

    auto allSeries = m_dbmanager->getSeries();
    foreach (auto series, allSeries) {
        if (!updateEndedSeries) {
            if (series["status"] != "Ended") {
               m_seriesIds.append(series["id"].toString());
            }
        } else {
             m_seriesIds.append(series["id"].toString());
        }
    }

    qDebug() << "m_seriesIds" << m_seriesIds;

    updateSeries();
}
