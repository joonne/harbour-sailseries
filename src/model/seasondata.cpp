#include "seasondata.h"

SeasonData::SeasonData(QObject *parent, int seasonNumber, QString seasonBanner, int watchedCount, int totalCount) :
    QObject(parent)
{
    m_seasonNumber = seasonNumber;
    m_seasonBanner = seasonBanner;
    m_watchedCount = watchedCount;
    m_totalCount = totalCount;
}

SeasonData::~SeasonData() {}

int SeasonData::getSeasonNumber() { return m_seasonNumber; }

QString SeasonData::getSeasonBanner() { return m_seasonBanner; }

int SeasonData::getWatchedCount() { return m_watchedCount; }

int SeasonData::getTotalCount() { return m_totalCount; }
