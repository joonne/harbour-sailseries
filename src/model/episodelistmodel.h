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

    Q_INVOKABLE void getEpisodes(QString seriesId, int seasonNumber);
    Q_INVOKABLE void toggleWatched(QString episodeId, QString seriesId, int seasonNumber);
    Q_INVOKABLE void markSeasonWatched(QString seriesId, int season);
    Q_INVOKABLE int getSeasonCount(QString seriesId);

signals:
    void episodeListChanged();
    void getEpisodesRequested(int seriesId, int seasonNumber);
    void toggleWatchedRequested(QString episodeId, QString seriesId, int seasonNumber);

public slots:
    void populateEpisodeList(QList<QMap<QString, QString> > episodes);

private:
    QList<EpisodeData*> m_episodeListModel;
    DatabaseManager* m_dbmanager;

    static int episodeListCount(QQmlListProperty<EpisodeData> *prop);
    static EpisodeData* episodeListAt(QQmlListProperty<EpisodeData> *prop, int index);
    static void episodeListAppend(QQmlListProperty<EpisodeData>* prop, EpisodeData* val);
    static void episodeListClear(QQmlListProperty<EpisodeData>* prop);

};

#endif // EPISODELISTMODEL_H
