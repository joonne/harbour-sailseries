#ifndef EPISODELISTMODEL_H
#define EPISODELISTMODEL_H

#include <QObject>
#include <QQmlContext>
#include <QDebug>
#include <QQmlListProperty>

#include "baselistmodel.h"
#include "../databasemanager.h"
#include "episodedata.h"

class EpisodeListModel : public QObject, public BaseListModel<EpisodeData>
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<EpisodeData> episodeList READ getList NOTIFY episodeListChanged)
public:
    explicit EpisodeListModel(QObject *parent = 0, DatabaseManager *dbmanager = 0);

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
    void setWatchedInModel(const int &episodeId, const bool &watched);

private:
    DatabaseManager* m_dbmanager;
};

#endif // EPISODELISTMODEL_H
