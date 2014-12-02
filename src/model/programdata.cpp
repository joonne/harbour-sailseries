#include "programdata.h"
#include <QDebug>

ProgramData::ProgramData(QObject *parent) :
    QObject(parent)
{
}

ProgramData::ProgramData(QObject *parent, QString programName, QString weekday, QString time):
    QObject(parent){

    myProgramName = programName;
    myWeekday = weekday;
    myTime = time;

    qDebug() << "creating program: " <<  programName;
}

QString ProgramData::getProgramName() { return myProgramName; }

QString ProgramData::getWeekday() { return myWeekday; }

QString ProgramData::getTime() { return myTime; }
