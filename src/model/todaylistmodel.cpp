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

    QList<QList<QString> > allSeries = mydbmanager->getStartPageSeries();
    for(int i = 0; i < allSeries.size(); ++i ) {
        QList<QString> temp = allSeries.at(i);

        if(temp.at(3) == today and temp.at(4) == "Continuing" and temp.size() > 5) {

            QString seriesName = temp.at(0);
            QString network = temp.at(1);
            QString airstime = temp.at(2);
            QString episodeName = temp.at(5);
            QString episodeNumber = temp.at(6);
            QString seasonNumber = temp.at(7);

            SeriesData* serie = new SeriesData(this,seriesName,network,airstime,
                                               episodeName,episodeNumber,seasonNumber);
            myTodayListModel.append(serie);

        }
    }

    emit todayModelChanged();
}

