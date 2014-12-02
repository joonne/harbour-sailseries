#include "programlistmodel.h"

ProgramListModel::ProgramListModel(QObject *parent) :
    QObject(parent)
{
    isPopulated = false;

}

QQmlListProperty<ProgramData> ProgramListModel::getProgramList() {

    return QQmlListProperty<ProgramData>(this,&myProgramListModel,&ProgramListModel::ProgramListCount,&ProgramListModel::ProgramListAt);

}

void ProgramListModel::ProgramListAppend(QQmlListProperty<ProgramData>* prop, ProgramData* val)
{
    ProgramListModel* seriesModel = qobject_cast<ProgramListModel*>(prop->object);
    seriesModel->myProgramListModel.append(val);
}

ProgramData* ProgramListModel::ProgramListAt(QQmlListProperty<ProgramData>* prop, int index)
{
    return (qobject_cast<ProgramListModel*>(prop->object))->myProgramListModel.at(index);
}

int ProgramListModel::ProgramListCount(QQmlListProperty<ProgramData>* prop)
{
    return qobject_cast<ProgramListModel*>(prop->object)->myProgramListModel.size();
}

void ProgramListModel::ProgramListClear(QQmlListProperty<ProgramData>* prop)
{
    qobject_cast<ProgramListModel*>(prop->object)->myProgramListModel.clear();
}

//void ProgramListModel::populatePrograms(int channelnumber) {

//    qDebug() << "creating channel number " << channelnumber;

//    if(myPrograms.size() != 0) {

//        QList<QMap<QString,QString> > channel = myPrograms.at(channelnumber-1);

//        qDebug() << "programs in channel " << channelnumber << ": " << channel.size();

//        for(int j = 0; j < channel.size(); ++j) {

//            QMap<QString,QString> program = channel.at(j);

//            QString programName = program["programName"];
//            QString weekday = program["weekday"];
//            QString time = program["time"];
//            myChannel = program["channel"];

//            ProgramData* programdata = new ProgramData(this,programName,weekday,time);

//            myProgramListModel.append(programdata);
//        }

//        emit programListChanged();
//        isPopulated = true;

//    } else {
//        qDebug() << "ei tietoja kanavista";
//    }

//}

void ProgramListModel::populatePrograms(QList<QMap<QString, QString> > programs) {

    if(!isPopulated) {

        for(int j = 0; j < programs.size(); ++j) {

            QMap<QString,QString> program = programs.at(j);

            QString programName = program["programName"];
            QString weekday = program["weekday"];
            QString time = program["time"];
            myChannel = program["channel"];

            ProgramData* programdata = new ProgramData(this,programName,weekday,time);

            myProgramListModel.append(programdata);
        }

        emit programListChanged();
        isPopulated = true;

    }
}

QString ProgramListModel::getChannel() { return myChannel; }
