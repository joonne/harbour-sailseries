#include "serieslistmodel.h"
// #include "sailfishapp.h"
#include "qcoreapplication.h"

SeriesListModel::SeriesListModel(QObject *parent, QQmlContext* context, DatabaseManager* dbmanager, XMLReader *reader) :
    QObject(parent),
    myContext(context){

    myReader = reader;
    mydbmanager = dbmanager;

    posterIndex = 0;

    mode = "default";

    connect(myReader,
            SIGNAL(readyToUpdateSeries()),
            this,
            SLOT(updateFetchFinished()));

    myLoading = false;

}

SeriesListModel::~SeriesListModel() {

    foreach(SeriesData* series, mySeriesListModel) {
        delete series;
        series = 0;
    }

    qDebug() << "destructing SeriesListModel";
}

void SeriesListModel::updateFetchFinished() {

    storeSeries();
    storeEpisodes();
    setLoading(false);
    populateBannerList();
}

QQmlListProperty<SeriesData> SeriesListModel::getSeriesList() {

    return QQmlListProperty<SeriesData>(this,&mySeriesListModel,&SeriesListModel::seriesListCount,&SeriesListModel::seriesListAt);

}

void SeriesListModel::seriesListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val)
{
    SeriesListModel* seriesModel = qobject_cast<SeriesListModel*>(prop->object);
    seriesModel->mySeriesListModel.append(val);
}

SeriesData* SeriesListModel::seriesListAt(QQmlListProperty<SeriesData>* prop, int index)
{
    return (qobject_cast<SeriesListModel*>(prop->object))->mySeriesListModel.at(index);
}

int SeriesListModel::seriesListCount(QQmlListProperty<SeriesData>* prop)
{
    return qobject_cast<SeriesListModel*>(prop->object)->mySeriesListModel.size();
}

void SeriesListModel::seriesListClear(QQmlListProperty<SeriesData>* prop)
{
    qobject_cast<SeriesListModel*>(prop->object)->mySeriesListModel.clear();
}

// -------------------------------------------------------------------
// POPULATING DATA

void SeriesListModel::populateSeriesList() {

    if(mySeries.size() != 0) {

        qDebug("getSeries works.");

        for(int i = 0; i < mySeries.size(); ++i) {

            qDebug() << mySeries.size();

            // This is the basic data
            QMap<QString,QString> temp = mySeries.at(i);
            QString seriesid = temp["seriesid"];
            QString language = temp["Langugage"];
            QString seriesName = temp["SeriesName"];
            QString aliasNames = temp["AliasNames"];
            QString banner = temp["banner"];
            QString overview = temp["Overview"];
            QString firstAired = temp["FirstAired"];
            QString imdb_id = temp["IMDB_ID"];
            QString zap2it_id = temp["zap2it_id"];
            QString network = temp["Network"];

            SeriesData* serie = new SeriesData(this, seriesid, language,
                                               seriesName, aliasNames,
                                               banner, overview, firstAired,
                                               imdb_id, zap2it_id, network);


            mySeriesListModel.append(serie);

        }


        // must remember to call signal to let QML side know about populated items..
        emit seriesListChanged();

        // mark flag
        isPopulated = true;

    }
}

void SeriesListModel::populateBannerList() {

    mySeriesListModel.clear();
    myPosters.clear();

    QList<QList<QString> > allSeries = mydbmanager->getSeries();
    for(int i = 0; i < allSeries.size(); ++i) {
        QList<QString> temp = allSeries.at(i);
        QString banner = temp.at(0);
        QString poster = temp.at(1);
        QString seriesName = temp.at(2);
        QString status = temp.at(3);
        QString id = temp.at(4);
        QString overview = temp.at(5);
        QString imdbID = temp.at(6);
        QString rating = temp.at(7);
        QString watchedCount = temp.at(8);
        QString totalCount = temp.at(9);

        QMap<QString,QString> nextEpisodeDetails = mydbmanager->getNextEpisodeDetails(id.toInt());
        QString episodeName = nextEpisodeDetails["episodeName"];
        QString episodeNumber = nextEpisodeDetails["episodeNumber"];
        QString seasonNumber = nextEpisodeDetails["seasonNumber"];
        QString daysto = nextEpisodeDetails["daysToNext"];

        myPosters.append(poster); // this is for the cover !
        SeriesData* serie = new SeriesData(this,banner,poster,seriesName,status,id,overview,
                                           imdbID,rating,episodeName,episodeNumber,seasonNumber,daysto,
                                           watchedCount, totalCount);
        mySeriesListModel.append(serie);
    }
    emit seriesListChanged();
}

void SeriesListModel::selectSeries(int index) {

    myInfo = mySeriesListModel.at(index);

    myPoster = myInfo->getPoster();
    emit posterChanged();

    myNextEpisodeName = myInfo->getNextEpisodeName();
    emit nextEpisodeNameChanged();

    myNextEpisodeNumber = myInfo->getNextEpisodeNumber();
    emit nextEpisodeNumberChanged();

    myNextEpisodeSeasonNumber = myInfo->getNextEpisodeSeasonNumber();
    emit nextEpisodeSeasonNumberChanged();

    myDaysToNextEpisode = myInfo->getDaysToNextEpisode();
    emit daysToNextEpisodeChanged();

}

void SeriesListModel::storeSeries() {

    mySeries = myReader->getSeries();
    QMap<QString,QString> temp = mySeries.first();

    int seriesid;
    QString actors;
    QString airsDayOfWeek;
    QString airsTime;
    QString contentRating;
    QString firstAired;
    QString genre;
    QString imdb_id;
    QString language;
    QString network;
    QString overview;
    double rating;
    int ratingCount;
    int runtime;
    QString SeriesName;
    QString status;
    QString added;
    int addedby;
    QString banner;
    QString fanart;
    QString lastUpdated;
    QString poster;
    QString zap2itid;
    // int watched;


    QMap<QString,QString>::iterator itr = temp.begin();
    while(itr != temp.end()) {

        if(itr.key() == "id") {
            seriesid = itr.value().toInt();
        } else if(itr.key() == "Actors") {
            actors = itr.value();
        } else if(itr.key() == "Airs_DayOfWeek") {
            airsDayOfWeek = itr.value();
        } else if(itr.key() == "Airs_Time") {
            airsTime = itr.value();
        } else if(itr.key() == "ContentRating") {
            contentRating = itr.value();
        } else if(itr.key() == "FirstAired") {
            firstAired = itr.value();
        } else if(itr.key() == "Genre") {
            genre = itr.value();
        } else if(itr.key() == "IMDB_ID") {
            imdb_id = itr.value();
        } else if(itr.key() == "Language") {
            language = itr.value();
        } else if(itr.key() == "Network") {
            network = itr.value();
        } else if(itr.key() == "Overview") {
            overview = itr.value();
        } else if(itr.key() == "Rating") {
            rating = itr.value().toDouble();
        } else if(itr.key() == "RatingCount") {
            ratingCount = itr.value().toInt();
        } else if(itr.key() == "Runtime") {
            runtime = itr.value().toInt();
        } else if(itr.key() == "SeriesName") {
            SeriesName = itr.value();
        } else if(itr.key() == "Status") {
            status = itr.value();
        } else if(itr.key() == "added") {
            added = itr.value();
        } else if(itr.key() == "addedBy") {
            addedby = itr.value().toInt();
        } else if(itr.key() == "banner") {
            banner = itr.value();
        } else if(itr.key() == "fanart") {
            fanart = itr.value();
        } else if(itr.key() == "lastupdated") {
            lastUpdated = itr.value();
        } else if(itr.key() == "poster") {
            poster = itr.value();
        } else if(itr.key() == "zap2it_id") {
            zap2itid = itr.value();
        }

        ++itr;
    }

    mydbmanager->insertSeries(seriesid,actors,airsDayOfWeek,airsTime,
                              contentRating,firstAired,genre,
                              imdb_id,language,network,overview,
                              rating,ratingCount,runtime,SeriesName,
                              status,added,addedby,banner,fanart,lastUpdated,
                              poster,zap2itid,0);
}

void SeriesListModel::storeEpisodes() {

    myEpisodes = myReader->getEpisodes();

    for(int i = 0; i < myEpisodes.size(); ++i) {

        QMap<QString,QString> temp = myEpisodes.at(i);

        int id = 0;
        QString director = "";
        int epimgflag = 0;
        QString episodeName = "";
        int episodeNumber = 0;
        QString firstAired = "";
        QString guestStars;
        QString imdb_id;
        QString language;
        QString overview;
        int productionCode;
        double rating;
        int ratingCount;
        int seasonNumber;
        QString writer;
        int absoluteNumber;
        int airsAfterSeason;
        int airsBeforeEpisode;
        int airsBeforeSeason;
        QString filename;
        QString lastUpdated;
        int seasonID;
        int seriesID;
        QString thumbAdded;
        int thumbHeight;
        int thumbWidth;

        QMap<QString,QString>::iterator itr = temp.begin();
        while(itr != temp.end()) {

            if(itr.key() == "id") {
                id = itr.value().toInt();
            } else if(itr.key() == "Director") {
                director = itr.value();
            } else if(itr.key() == "EpImgFlag") {
                epimgflag = itr.value().toInt();
            } else if(itr.key() == "EpisodeName") {
                episodeName = itr.value();
            } else if(itr.key() == "EpisodeNumber") {
                episodeNumber = itr.value().toInt();
            } else if(itr.key() == "FirstAired") {
                firstAired = itr.value();
            } else if(itr.key() == "GuestStars") {
                guestStars = itr.value();
            } else if(itr.key() == "Language") {
                language = itr.value();
            } else if(itr.key() == "Overview") {
                overview = itr.value();
            } else if(itr.key() == "IMDB_ID") {
                imdb_id = itr.value();
            } else if(itr.key() == "ProductionCode") {
                productionCode = itr.value().toInt();
            } else if(itr.key() == "Rating") {
                rating = itr.value().toDouble();
            } else if(itr.key() == "RatingCount") {
                ratingCount = itr.value().toInt();
            } else if(itr.key() == "SeasonNumber") {
                seasonNumber = itr.value().toInt();
            } else if(itr.key() == "Writer") {
                writer = itr.value();
            } else if(itr.key() == "absolute_number") {
                absoluteNumber = itr.value().toInt();
            } else if(itr.key() == "airsafter_season") {
                airsAfterSeason = itr.value().toInt();
            } else if(itr.key() == "airsbefore_episode") {
                airsBeforeEpisode = itr.value().toInt();
            } else if(itr.key() == "airsbefore_season") {
                airsBeforeSeason = itr.value().toInt();
            } else if(itr.key() == "filename") {
                filename = itr.value();
            } else if(itr.key() == "lastupdated") {
                lastUpdated = itr.value();
            } else if(itr.key() == "seasonid") {
                seasonID = itr.value().toInt();
            } else if(itr.key() == "seriesid") {
                seriesID = itr.value().toInt();
            } else if(itr.key() == "thumb_added") {
                thumbAdded = itr.value();
            } else if(itr.key() == "thumb_height") {
                thumbHeight = itr.value().toInt();
            } else if(itr.key() == "thumb_width") {
                thumbWidth = itr.value().toInt();
            }

            ++itr;
        }
        mydbmanager->insertEpisode(id,director,epimgflag,episodeName,episodeNumber,firstAired,
                                   guestStars,imdb_id,language,overview,productionCode,rating,
                                   ratingCount,seasonNumber,writer,absoluteNumber,airsAfterSeason,
                                   airsBeforeEpisode,airsBeforeSeason,filename,lastUpdated,seasonID,seriesID,
                                   thumbAdded,thumbHeight,thumbWidth);

        // process pending events to not block UI
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    }
}

QString SeriesListModel::getID() { return myInfo->getID(); }

QString SeriesListModel::getLanguage() { return myInfo->getLanguage(); }

QString SeriesListModel::getSeriesName() { return myInfo->getSeriesName(); }

QString SeriesListModel::getAliasNames() { return myInfo->getAliasNames(); }

QString SeriesListModel::getBanner() { return myInfo->getBanner(); }

QString SeriesListModel::getOverview() { return myInfo->getOverview(); }

QString SeriesListModel::getFirstAired() { return myInfo->getFirstAired(); }

QString SeriesListModel::getIMDB_ID() { return myInfo->getIMDB_ID(); }

QString SeriesListModel::getZap2it_ID() { return myInfo->getZap2it_ID(); }

QString SeriesListModel::getNetwork() { return myInfo->getNetwork(); }

QString SeriesListModel::getNextEpisodeName() { return myNextEpisodeName; }

QString SeriesListModel::getNextEpisodeNumber() { return myNextEpisodeNumber; }

QString SeriesListModel::getNextEpisodeSeasonNumber() { return myNextEpisodeSeasonNumber; }

QString SeriesListModel::getDaysToNextEpisode() { return myDaysToNextEpisode; }

QString SeriesListModel::getStatus() { return myInfo->getStatus(); }

QString SeriesListModel::getRating() { return myInfo->getRating(); }

bool SeriesListModel::getLoading() { return myLoading; }

void SeriesListModel::setLoading(bool) {

    if(myLoading) {
        myLoading = false;
    } else {
        myLoading = true;
    }
    emit loadingChanged();
}

QString SeriesListModel::getPoster() {

    qDebug() << myPoster;
    return myPoster;

//    if(myPosters.size() != 0) {
//        return myPosters.at(posterIndex);
//    } else {
//        return "";
//    }

    //return myInfo->getPoster();
}

void SeriesListModel::setPoster(QString) {
//    ++posterIndex;
//    emit posterChanged();
}

void SeriesListModel::nextPoster() {

    if(posterIndex == myPosters.size()-1) {
        posterIndex = 0;
    } else {
        ++posterIndex;
    }

    emit posterChanged();

}

QString SeriesListModel::getMode() {
    return mode;
}

void SeriesListModel::setMode(QString newmode) {

    if(mode !=  newmode) {
        mode = newmode;
        emit modeChanged();
        qDebug() << "mode changed!";
    }
}

void SeriesListModel::deleteSeries(int seriesID) {

    mydbmanager->setUpDB();
    myLoading = true;
    if(mydbmanager->deleteSeries(seriesID)) {
        populateBannerList();
        emit updateModels();
    }
    myLoading = false;
}

void SeriesListModel::updateSeries(QString seriesID) {

    myReader->getFullSeriesRecord(seriesID,"update");
    setLoading(true);
}




