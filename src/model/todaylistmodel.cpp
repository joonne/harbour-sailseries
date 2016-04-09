#include "todaylistmodel.h"

TodayListModel::TodayListModel(QObject *parent, DatabaseManager *dbmanager, XMLReader *reader) :
    QObject(parent)
{
    mydbmanager = dbmanager;
    myReader = reader;

    populateTodayModel();
}

TodayListModel::~TodayListModel() {

    foreach(SeriesData* series, myTodayListModel) {
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

    QDate date = QDateTime::currentDateTime().date();
    QLocale locale  = QLocale(QLocale::English);
    QString today = locale.toString(date, "dddd");

    myTodayListModel.clear();
    emit todayModelChanged();

    QList<QMap<QString, QString> > allSeries = mydbmanager->getStartPageSeries();
    int length = allSeries.size();
    for(int i = 0; i < length; ++i ) {

        QMap<QString, QString> temp = allSeries.at(i);

        if(temp["airsDayOfWeek"] == today and temp["status"] == "Continuing" and temp.contains("episodeName")) {
            SeriesData* series = new SeriesData(this, temp);
            myTodayListModel.append(series);
        }
    }

    emit todayModelChanged();
}

