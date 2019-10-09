#include "todaylistmodel.h"

TodayListModel::TodayListModel(QObject *parent, DatabaseManager *dbmanager, Api *api) :
    QObject(parent)
{
    m_dbmanager = dbmanager;
    m_api = api;

    connect(this,
            SIGNAL(getStartPageSeries()),
            m_dbmanager,
            SLOT(getStartPageSeries()));

    connect(m_dbmanager,
            SIGNAL(populateTodayModel(QList<QVariantMap>)),
            this,
            SLOT(populateTodayModel(QList<QVariantMap>)));

    emit getStartPageSeries();
}

TodayListModel::~TodayListModel()
{
    for (auto series : m_todayListModel) {
        delete series;
        series = 0;
    }
}

QQmlListProperty<SeriesData> TodayListModel::getTodayModel()
{
    return QQmlListProperty<SeriesData>(this, &m_todayListModel, &TodayListModel::todayListCount, &TodayListModel::todayListAt);
}

void TodayListModel::todayListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val)
{
    TodayListModel* todayModel = qobject_cast<TodayListModel*>(prop->object);
    todayModel->m_todayListModel.append(val);
}

SeriesData* TodayListModel::todayListAt(QQmlListProperty<SeriesData>* prop, int index)
{
    return (qobject_cast<TodayListModel*>(prop->object))->m_todayListModel.at(index);
}

int TodayListModel::todayListCount(QQmlListProperty<SeriesData>* prop)
{
    return qobject_cast<TodayListModel*>(prop->object)->m_todayListModel.size();
}

void TodayListModel::todayListClear(QQmlListProperty<SeriesData>* prop)
{
    qobject_cast<TodayListModel*>(prop->object)->m_todayListModel.clear();
}

void TodayListModel::populateTodayModel(QList<QVariantMap> allSeries)
{
    m_todayListModel.clear();
    emit todayModelChanged();

    for (auto series : allSeries) {
        auto seriesData = new SeriesData(this, series);
        m_todayListModel.append(seriesData);
    }
    emit todayModelChanged();
}

