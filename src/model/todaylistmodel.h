#ifndef TODAYLISTMODEL_H
#define TODAYLISTMODEL_H

#include <QObject>
#include <QDebug>
#include <QQmlListProperty>
#include <QDate>
#include <QLocale>

#include "databasemanager.h"
#include "api.h"
#include "model/seriesdata.h"

class TodayListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<SeriesData> TodayModel READ getTodayModel NOTIFY todayModelChanged)
public:
    explicit TodayListModel(QObject *parent = 0, DatabaseManager* dbmanager = 0);
    ~TodayListModel();

    QQmlListProperty<SeriesData> getTodayModel();

signals:
    void todayModelChanged();
    void getStartPageSeries();

public slots:
    void populateTodayModel(QList<QVariantMap> allSeries);

private:
    DatabaseManager* m_dbmanager;
    Api* m_api;
    QList<SeriesData*> m_todayListModel;

    static int todayListCount(QQmlListProperty<SeriesData> *prop);
    static SeriesData* todayListAt(QQmlListProperty<SeriesData> *prop, int index);
    static void todayListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val);
    static void todayListClear(QQmlListProperty<SeriesData>* prop);
};

#endif // TODAYLISTMODEL_H
