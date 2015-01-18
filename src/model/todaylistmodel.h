#ifndef TODAYLISTMODEL_H
#define TODAYLISTMODEL_H

#include <QObject>
#include <QDebug>
#include <QQmlListProperty>
#include <QDate>
#include <QLocale>

#include "databasemanager.h"
#include "xmlreader.h"
#include "model/seriesdata.h"

class TodayListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<SeriesData> TodayModel READ getTodayModel NOTIFY todayModelChanged)
public:
    explicit TodayListModel(QObject *parent = 0, DatabaseManager* dbmanager = 0, XMLReader* reader = 0);
    ~TodayListModel();

    Q_INVOKABLE void populateTodayModel();

    QQmlListProperty<SeriesData> getTodayModel();

signals:
    void todayModelChanged();

public slots:

private:
    DatabaseManager* mydbmanager;
    XMLReader* myReader;
    QList<QMap<QString,QString> > mySeries;
    QList<QMap<QString,QString> > myEpisodes;
    QList<SeriesData*> myTodayListModel;

    static int todayListCount(QQmlListProperty<SeriesData> *prop);
    static SeriesData* todayListAt(QQmlListProperty<SeriesData> *prop, int index);
    static void todayListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val);
    static void todayListClear(QQmlListProperty<SeriesData>* prop);

};

#endif // TODAYLISTMODEL_H
