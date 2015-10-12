#ifndef EPISODELISTMODEL_H
#define EPISODELISTMODEL_H

#include <QObject>
#include <QQmlContext>
#include <QDebug>
#include <QQmlListProperty>
#include <QDate>

#include "../databasemanager.h"
#include "episodedata.h"

class EpisodeListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<EpisodeData> episodeList READ getEpisodeList NOTIFY episodeListChanged)
public:
    explicit EpisodeListModel(QObject *parent = 0,DatabaseManager* dbmanager = 0);
    ~EpisodeListModel();

    QQmlListProperty<EpisodeData> getEpisodeList();

    Q_INVOKABLE void populateEpisodeList(QString seriesID, int seasonNumber);
    Q_INVOKABLE void toggleWatched(QString episodeID);
    Q_INVOKABLE void markSeasonWatched(QString seriesID, int season);
    Q_INVOKABLE int getSeasonCount(QString seriesID);
signals:
    void episodeListChanged();

public slots:

private:
    QList<EpisodeData*> myEpisodeListModel;
    DatabaseManager* mydbmanager;

    static int episodeListCount(QQmlListProperty<EpisodeData> *prop);
    static EpisodeData* episodeListAt(QQmlListProperty<EpisodeData> *prop, int index);
    static void episodeListAppend(QQmlListProperty<EpisodeData>* prop, EpisodeData* val);
    static void episodeListClear(QQmlListProperty<EpisodeData>* prop);

};

#endif // EPISODELISTMODEL_H
