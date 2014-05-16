#include "seriesdata.h"

SeriesData::SeriesData(QObject *parent) :
    QObject(parent)
{
}

SeriesData::SeriesData(QObject *parent, QString ID, QString language, QString seriesName,
                       QString aliasNames, QString banner, QString firstAired,
                       QString IMDB_ID, QString zap2it_ID, QString network):
    QObject(parent)
{

    myID = ID;
    myLanguage = language;
    mySeriesName = seriesName;
    myAliasNames = aliasNames;
    myBanner = banner;
    myFirstAired = firstAired;
    myIMDB_ID = IMDB_ID;
    myZap2it_ID = zap2it_ID;
    myNetwork = network;
}

QString SeriesData::getID() { return myID; }

QString SeriesData::getLanguage() { return myLanguage; }

QString SeriesData::getSeriesName() { return mySeriesName; }

QString SeriesData::getAliasNames() { return myAliasNames; }

QString SeriesData::getBanner() { return myBanner; }

QString SeriesData::getFirstAired() { return myFirstAired; }

QString SeriesData::getIMDB_ID() { return myIMDB_ID; }

QString SeriesData::getZap2it_ID() { return myZap2it_ID; }

QString SeriesData::getNetwork() { return myNetwork; }
