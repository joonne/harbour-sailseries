#include "episodelistmodel.h"

EpisodeListModel::EpisodeListModel(QObject *parent, DatabaseManager *dbmanager) :
    QObject(parent)
{
    mydbmanager = dbmanager;
}

EpisodeListModel::~EpisodeListModel() {

    foreach(EpisodeData* episode, myEpisodeListModel) {
        delete episode;
        episode = 0;
    }

    qDebug() << "destructing EpisodeListModel";
}

QQmlListProperty<EpisodeData> EpisodeListModel::getEpisodeList() {

    return QQmlListProperty<EpisodeData>(this,&myEpisodeListModel,&EpisodeListModel::episodeListCount,&EpisodeListModel::episodeListAt);

}

// list handling methods

void EpisodeListModel::episodeListAppend(QQmlListProperty<EpisodeData>* prop, EpisodeData* val)
{
    EpisodeListModel* episodeModel = qobject_cast<EpisodeListModel*>(prop->object);
    episodeModel->myEpisodeListModel.append(val);
}

EpisodeData* EpisodeListModel::episodeListAt(QQmlListProperty<EpisodeData>* prop, int index)
{
    return (qobject_cast<EpisodeListModel*>(prop->object))->myEpisodeListModel.at(index);
}

int EpisodeListModel::episodeListCount(QQmlListProperty<EpisodeData>* prop)
{
    return qobject_cast<EpisodeListModel*>(prop->object)->myEpisodeListModel.size();
}

void EpisodeListModel::episodeListClear(QQmlListProperty<EpisodeData>* prop)
{
    qobject_cast<EpisodeListModel*>(prop->object)->myEpisodeListModel.clear();
}

void EpisodeListModel::populateEpisodeList(QString seriesID, int seasonNumber) {

    myEpisodeListModel.clear();
    emit episodeListChanged();

    QList<QMap<QString, QString> > episodes = mydbmanager->getEpisodes(seriesID.toInt(), seasonNumber);

    if(episodes.size() != 0) {

        int size = episodes.size();
        for(int i = 0; i < size; ++i) {

            QMap<QString, QString> temp = episodes.at(i);

            // we don't want the special episodes, they have season number 0.
            if(seasonNumber != 0 ) {
                EpisodeData* episode = new EpisodeData(this, temp);
                myEpisodeListModel.append(episode);
            }
        }

        // must remember to call signal to let QML side know about populated items
        emit episodeListChanged();
    }
}

void EpisodeListModel::toggleWatched(QString episodeID) {

    mydbmanager->toggleWatched(episodeID);
}

void EpisodeListModel::markSeasonWatched(QString seriesID, int season) {

    mydbmanager->markSeasonWatched(seriesID.toInt(), season);
    populateEpisodeList(seriesID, season);
    emit episodeListChanged();
}

int EpisodeListModel::getSeasonCount(QString seriesID) {

    return mydbmanager->seasonCount(seriesID.toInt());
}
