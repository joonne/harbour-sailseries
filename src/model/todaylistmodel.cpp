#include "todaylistmodel.h"

TodayListModel::TodayListModel(QObject *parent, DatabaseManager *dbmanager, XMLReader *reader) :
    QObject(parent)
{
    mydbmanager = dbmanager;
    myReader = reader;

    populateTodayModel();
}

TodayListModel::~TodayListModel() {

    foreach(auto series, myTodayListModel) {
        delete series;
        series = 0;
    }
    qDebug() << "destructing TodayListModel";

}

QQmlListProperty<SeriesData> TodayListModel::getTodayModel() {

    return QQmlListProperty<SeriesData>(this,&myTodayListModel,&TodayListModel::todayListCount,&TodayListModel::todayListAt);

}

void TodayListModel::todayListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val) {

    TodayListModel* todayModel = qobject_cast<TodayListModel*>(prop->object);
    todayModel->myTodayListModel.append(val);
}

SeriesData* TodayListModel::todayListAt(QQmlListProperty<SeriesData>* prop, int index) {

    return (qobject_cast<TodayListModel*>(prop->object))->myTodayListModel.at(index);
}

int TodayListModel::todayListCount(QQmlListProperty<SeriesData>* prop) {
    return qobject_cast<TodayListModel*>(prop->object)->myTodayListModel.size();
}

void TodayListModel::todayListClear(QQmlListProperty<SeriesData>* prop) {
    qobject_cast<TodayListModel*>(prop->object)->myTodayListModel.clear();
}

void TodayListModel::populateTodayModel() {

    myTodayListModel.clear();
    emit todayModelChanged();

    auto allSeries = mydbmanager->getStartPageSeries();
    auto length = allSeries.size();
    for(auto i = 0; i < length; ++i ) {

        auto temp = allSeries.at(i);

        auto series = new SeriesData(this, temp);
        myTodayListModel.append(series);
    }

    emit todayModelChanged();
}

