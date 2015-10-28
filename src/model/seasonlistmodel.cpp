#include "seasonlistmodel.h"

SeasonListModel::SeasonListModel(QObject *parent,  DatabaseManager* dbmanager) :
    QObject(parent)
{
    mydbmanager = dbmanager;
}

SeasonListModel::~SeasonListModel() {

    foreach (SeasonData* season, mySeasonListModel) {
        delete season;
        season = 0;
    }
    qDebug() << "destructing SeasonListModel";
}

QQmlListProperty<SeasonData> SeasonListModel::getSeasonList() {
    return QQmlListProperty<SeasonData>(this, &mySeasonListModel, &SeasonListModel::seasonListCount, &SeasonListModel::seasonListAt);
}

// list handling methods

void SeasonListModel::seasonListAppend(QQmlListProperty<SeasonData>* prop, SeasonData* val)
{
    SeasonListModel* seasonModel = qobject_cast<SeasonListModel*>(prop->object);
    seasonModel->mySeasonListModel.append(val);
}

SeasonData* SeasonListModel::seasonListAt(QQmlListProperty<SeasonData>* prop, int index)
{
    return (qobject_cast<SeasonListModel*>(prop->object))->mySeasonListModel.at(index);
}

int SeasonListModel::seasonListCount(QQmlListProperty<SeasonData>* prop)
{
    return qobject_cast<SeasonListModel*>(prop->object)->mySeasonListModel.size();
}

void SeasonListModel::seasonListClear(QQmlListProperty<SeasonData>* prop)
{
    qobject_cast<SeasonListModel*>(prop->object)->mySeasonListModel.clear();
}

void SeasonListModel::populateSeasonList(QString seriesID) {

    qDebug() << "populating seasonlist " + seriesID;

    mySeasonListModel.clear();
    emit seasonListChanged();

    int seasonsCount = mydbmanager->seasonCount(seriesID.toInt());
    qDebug() << "seasonsCount: " + seasonsCount;
    int watchedCount = 0;
    int totalCount = 0;

    for(int i = 1; i <= seasonsCount; ++i) {

        watchedCount = mydbmanager->watchedCountBySeason(seriesID.toInt(),i);
        qDebug() << "watchedCount: " + watchedCount;
        totalCount = mydbmanager->totalCountBySeason(seriesID.toInt(),i);
        qDebug() << "totalCount: " + totalCount;

        SeasonData* season = new SeasonData(this, i, "", watchedCount, totalCount);
        mySeasonListModel.append(season);

    }
    emit seasonListChanged();
}
