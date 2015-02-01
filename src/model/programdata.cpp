#include "programdata.h"
#include <QDebug>

ProgramData::ProgramData(QObject *parent) :
    QObject(parent)
{
}

ProgramData::ProgramData(QObject *parent, QString programName, QString weekday,
                         QString time, QString channelName, QString overview):
    QObject(parent){

    myProgramName = programName;
    myWeekday = weekday;
    myTime = time;
    myChannelName = channelName;
    myOverview = overview;
}

QString ProgramData::getProgramName() { return myProgramName; }

QString ProgramData::getWeekday() { return myWeekday; }

QString ProgramData::getTime() { return myTime; }

QString ProgramData::getChannelName() { return myChannelName; }

QString ProgramData::getOverview() { return myOverview; }
