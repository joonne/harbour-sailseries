#ifndef SEASONLISTMODEL_H
#define SEASONLISTMODEL_H

#include <QObject>
#include <QDebug>
#include <QQmlListProperty>

#include "../databasemanager.h"
#include "seasondata.h"

class SeasonListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<SeasonData> SeasonList READ getSeasonList NOTIFY seasonListChanged)
public:
    explicit SeasonListModel(QObject *parent = 0, DatabaseManager* dbmanager = 0);
    ~SeasonListModel();

    QQmlListProperty<SeasonData> getSeasonList();

    Q_INVOKABLE void getSeasons(QString seriesId);

signals:
    void seasonListChanged();
    void getSeasonsRequested(int seriesId);

public slots:
    void populateSeasonList(QList<QVariantMap> seasons);

private:
    QList<SeasonData*> m_seasonListModel;
    DatabaseManager* m_dbmanager;

    static int seasonListCount(QQmlListProperty<SeasonData> *prop);
    static SeasonData* seasonListAt(QQmlListProperty<SeasonData> *prop, int index);
    static void seasonListAppend(QQmlListProperty<SeasonData>* prop, SeasonData* val);
    static void seasonListClear(QQmlListProperty<SeasonData>* prop);

};

#endif // SEASONLISTMODEL_H
