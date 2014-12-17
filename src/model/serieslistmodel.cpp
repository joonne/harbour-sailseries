#include "serieslistmodel.h"

seriesListModel::seriesListModel(QObject *parent, QQmlContext* context) :
    QObject(parent),
    myReader(new XMLReader),
    myContext(context),
    mydbmanager(new DatabaseManager){

    isPopulated = false;

    qDebug() << "connecting signal and slot";
    connect(myReader,
            SIGNAL(readyToPopulateSeries()),
            this,
            SLOT(xmlParseFinished()));

    connect(myReader,
            SIGNAL(readyToStoreSeries()),
            this,
            SLOT(getFullSeriesRecordFinished()));

    mydbmanager->setUpDB();

    myLoading = false;

    posterIndex = 0;

    mode = "default";

    //populateBannerList();
    populateTodayList();

}

seriesListModel::~seriesListModel() {
    //delete mySeriesListModel;
}

void seriesListModel::xmlParseFinished() {

    qDebug("slotti toimii");
    mySeries = myReader->getSeries();
    populateSeriesList();
}

QQmlListProperty<SeriesData> seriesListModel::getSeriesList() {

    return QQmlListProperty<SeriesData>(this,&mySeriesListModel,&seriesListModel::seriesListCount,&seriesListModel::seriesListAt);

}

void seriesListModel::seriesListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val)
{
    seriesListModel* seriesModel = qobject_cast<seriesListModel*>(prop->object);
    seriesModel->mySeriesListModel.append(val);
}

SeriesData* seriesListModel::seriesListAt(QQmlListProperty<SeriesData>* prop, int index)
{
    return (qobject_cast<seriesListModel*>(prop->object))->mySeriesListModel.at(index);
}

int seriesListModel::seriesListCount(QQmlListProperty<SeriesData>* prop)
{
    return qobject_cast<seriesListModel*>(prop->object)->mySeriesListModel.size();
}

void seriesListModel::seriesListClear(QQmlListProperty<SeriesData>* prop)
{
    qobject_cast<seriesListModel*>(prop->object)->mySeriesListModel.clear();
}

void seriesListModel::getFullSeriesRecordFinished() {
    storeSeries();
    storeEpisodes();
}

// -------------------------------------------------------------------
// POPULATING DATA

void seriesListModel::populateSeriesList() {


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

void seriesListModel::populateBannerList() {

    QList<QList<QString> > allSeries = mydbmanager->getSeries();
    qDebug() << "populating banners for " << allSeries.size() << " series.";
    for(int i = 0; i < allSeries.size(); ++i) {
        QList<QString> temp = allSeries.at(i);
        QString banner = temp.at(0);
        QString poster = temp.at(1);
        myPosters.append(poster); // this is for the cover !
        SeriesData* serie = new SeriesData(this,banner,poster);
        //SeriesData* serie = new SeriesData(this,"1","en","asd","asd",temp.at(i),"asd","asd","asd","asd","asd");
        mySeriesListModel.append(serie);
        qDebug() << "appended";
    }
    emit seriesListChanged();
    qDebug() << "signal emitted";
}

void seriesListModel::populateTodayList() {

    QDate d;
    QString today = d.currentDate().toString("dddd");
    qDebug() << today;

    mySeriesListModel.clear();
    emit seriesListChanged();

    QList<QList<QString> > allSeries = mydbmanager->getStartPageSeries();
    for(int i = 0; i < allSeries.size(); ++i ) {
        QList<QString> temp = allSeries.at(i);

        if(temp.at(3) == today and temp.at(4) == "Continuing") {

            QString seriesName = temp.at(0);
            QString network = temp.at(1);
            QString airstime = temp.at(2);

            SeriesData* serie = new SeriesData(this,seriesName,network,airstime);
            mySeriesListModel.append(serie);

        }
    }

    emit seriesListChanged();
}

void seriesListModel::searchSeries(QString text) {

    mySeriesListModel.clear();
    mySeries.clear();
    emit seriesListChanged();
    myReader->searchSeries(text);
}

void seriesListModel::selectSeries(int index) {

    myInfo = mySeriesListModel.at(index);
    qDebug() << "myInfo seriesname: " << myInfo->getSeriesName();

}

void seriesListModel::getFullSeriesRecord(QString id) {
    myReader->getFullSeriesRecord(id);
    setLoading(true);

}

void seriesListModel::storeSeries() {

    mySeries = myReader->getSeries();
    QMap<QString,QString> temp = mySeries.at(0);

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

        if(itr.key() == "seriesid") {
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

void seriesListModel::storeEpisodes() {

    myEpisodes = myReader->getEpisodes();

    for(int i = 0; i < myEpisodes.size(); ++i) {

        QMap<QString,QString> temp = myEpisodes.at(i);

        int id;
        QString director;
        int epimgflag;
        QString episodeName;
        int episodeNumber;
        QString firstAired;
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
        //int watched;

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
                                   thumbAdded,thumbHeight,thumbWidth,0);
    }

    setLoading(false);
}

QString seriesListModel::getID() { return myInfo->getID(); }

QString seriesListModel::getLanguage() { return myInfo->getLanguage(); }

QString seriesListModel::getSeriesName() { return myInfo->getSeriesName(); }

QString seriesListModel::getAliasNames() { return myInfo->getAliasNames(); }

QString seriesListModel::getBanner() { return myInfo->getBanner(); }

QString seriesListModel::getOverview() { return myInfo->getOverview(); }

QString seriesListModel::getFirstAired() { return myInfo->getFirstAired(); }

QString seriesListModel::getIMDB_ID() { return myInfo->getIMDB_ID(); }

QString seriesListModel::getZap2it_ID() { return myInfo->getZap2it_ID(); }

QString seriesListModel::getNetwork() { return myInfo->getNetwork(); }

bool seriesListModel::getLoading() { return myLoading; }

void seriesListModel::setLoading(bool) {

    if(myLoading) {
        myLoading = false;
    } else {
        myLoading = true;
    }
    emit loadingChanged();
}

QString seriesListModel::getPoster() {

    if(myPosters.size() != 0) {
        return myPosters.at(posterIndex);
    } else {
        return "";
    }
}

void seriesListModel::setPoster(QString) {
    ++posterIndex;
    emit posterChanged();
}

void seriesListModel::nextPoster() {

    if(posterIndex == myPosters.size()-1) {
        posterIndex = 0;
    } else {
        ++posterIndex;
    }

    emit posterChanged();

}

QString seriesListModel::getMode() {
    return mode;
}

void seriesListModel::setMode(QString newmode) {

    if(mode !=  newmode) {
        mode = newmode;
        emit modeChanged();
    }
}

// jotain fiksumpaa tähän varmaankin
void seriesListModel::clearList() {
    mySeriesListModel.clear();
    emit seriesListChanged();
}




