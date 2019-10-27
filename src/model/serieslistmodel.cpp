#include "serieslistmodel.h"

SeriesListModel::SeriesListModel(QObject *parent, DatabaseManager* dbmanager, Api *api) :
    QObject(parent),
    m_api(api),
    m_dbmanager(dbmanager),
    m_mode("default"),
    m_isLoading(false)
{
    connect(this,
            SIGNAL(getSeries()),
            m_dbmanager,
            SLOT(getSeries()));

    connect(m_dbmanager,
            SIGNAL(populateBannerList(QList<QVariantMap>)),
            this,
            SLOT(populateBannerList(QList<QVariantMap>)));

    connect(this,
            SIGNAL(deleteSeriesWith(int)),
            m_dbmanager,
            SLOT(deleteSeries(int)));

    connect(m_dbmanager,
            SIGNAL(seriesDeleted()),
            this,
            SLOT(seriesDeleted()));

    connect(m_dbmanager,
            SIGNAL(seriesStored()),
            this,
            SLOT(seriesStored()));

    emit getSeries();
}

SeriesListModel::~SeriesListModel()
{
    for (auto series : m_seriesListModel)
    {
        delete series;
        series = 0;
    }
}

void SeriesListModel::seriesStored()
{
    setLoading(false);
    emit getSeries();
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

void SeriesListModel::populateBannerList(QList<QVariantMap> allSeries)
{
    m_seriesListModel.clear();
    emit seriesListChanged();

    for (auto series : allSeries)
    {
        const auto seriesData = new SeriesData(this, series);
        m_seriesListModel.append(seriesData);
    }

    emit seriesListChanged();
}

void SeriesListModel::selectSeries(int index)
{
    m_info = m_seriesListModel.at(index);
    setMode("m_series");

    emit posterChanged();
    emit nextEpisodeNameChanged();
    emit nextEpisodeNumberChanged();
    emit nextEpisodeSeasonNumberChanged();
    emit daysToNextEpisodeChanged();
}

int SeriesListModel::getID() { return m_info->getID(); }

QString SeriesListModel::getLanguage() { return m_info->getLanguage(); }

QString SeriesListModel::getSeriesName() { return m_info->getSeriesName(); }

QString SeriesListModel::getAliasNames() { return m_info->getAliasNames(); }

QString SeriesListModel::getBanner() { return m_info->getBanner(); }

QString SeriesListModel::getOverview() { return m_info->getOverview(); }

QString SeriesListModel::getFirstAired() { return m_info->getFirstAired(); }

QString SeriesListModel::getIMDB_ID() { return m_info->getIMDB_ID(); }

QString SeriesListModel::getZap2it_ID() { return m_info->getZap2it_ID(); }

QString SeriesListModel::getNetwork() { return m_info->getNetwork(); }

QString SeriesListModel::getNextEpisodeName() { return m_info->getNextEpisodeName(); }

QString SeriesListModel::getNextEpisodeNumber() { return m_info->getNextEpisodeNumber(); }

QString SeriesListModel::getNextEpisodeSeasonNumber() { return m_info->getNextEpisodeSeasonNumber(); }

QString SeriesListModel::getDaysToNextEpisode() { return m_info->getDaysToNextEpisode(); }

QString SeriesListModel::getStatus() { return m_info->getStatus(); }

QString SeriesListModel::getRating() { return m_info->getRating(); }

QString SeriesListModel::getGenre() { return m_info->getGenre(); }

bool SeriesListModel::getLoading() { return m_isLoading; }

void SeriesListModel::setLoading(bool isLoading)
{
    if (m_isLoading == isLoading)
    {
        return;
    }

    m_isLoading = isLoading;
    emit loadingChanged();
}

QString SeriesListModel::getPoster() { return m_info->getPoster(); }

QString SeriesListModel::getMode() { return m_mode; }

void SeriesListModel::setMode(QString mode)
{
    if (m_mode == mode)
    {
        return;
    }

    m_mode = mode;
    emit modeChanged();
}

void SeriesListModel::deleteSeries(int seriesId)
{
    setLoading(true);
    emit deleteSeriesWith(seriesId);
}

void SeriesListModel::seriesDeleted()
{
    emit updateModels();
    setLoading(false);
}

void SeriesListModel::updateSeries(const QString &seriesId)
{
    if (seriesId.isEmpty())
    {
        return;
    }

    m_api->getAll(seriesId.toInt());
    setLoading(true);
}

void SeriesListModel::updateAllSeries(bool includeEndedSeries)
{
    const auto seriesIds = m_dbmanager->getSeriesIds(includeEndedSeries);
    for (auto seriesId: seriesIds)
    {
        updateSeries(seriesId);
    }
}
