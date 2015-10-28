#include "seasondata.h"

SeasonData::SeasonData(QObject *parent, int seasonNumber, QString seasonBanner, int watchedCount, int totalCount) :
    QObject(parent)
{
    mySeasonNumber = seasonNumber;
    mySeasonBanner = seasonBanner;
    myWatchedCount = watchedCount;
    myTotalCount = totalCount;
}

SeasonData::~SeasonData() {}

int SeasonData::getSeasonNumber() {
    return mySeasonNumber;
}

QString SeasonData::getSeasonBanner() {
    return mySeasonBanner;
}

int SeasonData::getWatchedCount() {
    return myWatchedCount;
}

int SeasonData::getTotalCount() {
    return myTotalCount;
}
