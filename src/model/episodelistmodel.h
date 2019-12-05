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
    explicit EpisodeListModel(QObject *parent = 0, DatabaseManager *dbmanager = 0);
    ~EpisodeListModel();

    QQmlListProperty<EpisodeData> getEpisodeList();

    Q_INVOKABLE void getEpisodes(const int &seriesId, const int &seasonNumber);
    Q_INVOKABLE void setWatched(const int &episodeId, const int &seriesId, const bool &watched);
    Q_INVOKABLE void markSeasonAsWatched(const int &seriesId, const int &season);

signals:
    void episodeListChanged();
    void getEpisodesRequested(const int &seriesId, const int &seasonNumber);
    void setWatchedRequested(const int &episodeId, const int &seriesId, const bool &watched);
    void markSeasonAsWatchedRequested(const int &seriesId, const int &season);

public slots:
    void populateEpisodeList(const QList<QVariantMap> &episodes);

private:
    QList<EpisodeData*> m_episodeListModel;
    DatabaseManager* m_dbmanager;

    static int episodeListCount(QQmlListProperty<EpisodeData> *prop);
    static EpisodeData* episodeListAt(QQmlListProperty<EpisodeData> *prop, int index);
    static void episodeListAppend(QQmlListProperty<EpisodeData>* prop, EpisodeData* val);
    static void episodeListClear(QQmlListProperty<EpisodeData>* prop);

};

#endif // EPISODELISTMODEL_H
