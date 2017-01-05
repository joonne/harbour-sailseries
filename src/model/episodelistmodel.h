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
    Q_INVOKABLE void toggleWatched(QString episodeId);
    Q_INVOKABLE void markSeasonWatched(QString seriesID, int season);
    Q_INVOKABLE int getSeasonCount(QString seriesID);
signals:
    void episodeListChanged();

public slots:

private:
    QList<EpisodeData*> m_episodeListModel;
    DatabaseManager* m_dbmanager;

    static int episodeListCount(QQmlListProperty<EpisodeData> *prop);
    static EpisodeData* episodeListAt(QQmlListProperty<EpisodeData> *prop, int index);
    static void episodeListAppend(QQmlListProperty<EpisodeData>* prop, EpisodeData* val);
    static void episodeListClear(QQmlListProperty<EpisodeData>* prop);

};

#endif // EPISODELISTMODEL_H
