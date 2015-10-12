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

    QList<QList<QString> > episodes = mydbmanager->getEpisodes(seriesID.toInt(), seasonNumber);

    if(episodes.size() != 0) {

        for(int i = 0; i < episodes.size(); ++i) {

            QList<QString> temp = episodes.at(i);
            qDebug() << temp;

            QString episodeName = temp.at(0);
            int episodeNumber = temp.at(1).toInt();
            QString overview = temp.at(2);
            int seasonNumber = temp.at(3).toInt();
            int absoluteNumber = temp.at(4).toInt();
            QString filename = temp.at(5);
            int watched = temp.at(6).toInt();
            int id = temp.at(7).toInt();
            QString guestStars = temp.at(8);
            QString writer = temp.at(9);
            QString firstAired = temp.at(10);

            // we don't want the special episodes, they have season number 0.
            if(seasonNumber != 0 ) {

                EpisodeData* episode = new EpisodeData(this,
                                                       episodeName,
                                                       episodeNumber,
                                                       overview,
                                                       seasonNumber,
                                                       absoluteNumber,
                                                       filename,
                                                       watched,
                                                       id,
                                                       guestStars,
                                                       writer,
                                                       firstAired);

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
}

int EpisodeListModel::getSeasonCount(QString seriesID) {

    return mydbmanager->seasonCount(seriesID.toInt());
}
