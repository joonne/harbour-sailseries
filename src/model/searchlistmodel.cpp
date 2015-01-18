#include "searchlistmodel.h"

SearchListModel::SearchListModel(QObject *parent, DatabaseManager *dbmanager, XMLReader *xmlreader) :
    QObject(parent)
{
    mydbmanager = dbmanager;
    myReader = xmlreader;

    connect(myReader,
            SIGNAL(readyToPopulateSeries()),
            this,
            SLOT(xmlParseFinished()));

    connect(myReader,
            SIGNAL(readyToStoreSeries()),
            this,
            SLOT(getFullSeriesRecordFinished()));

    myLoading = false;
    myAdded = false;
}

SearchListModel::~SearchListModel() {

    mydbmanager->close();
    foreach(SeriesData* series, mySearchListModel) {
        delete series;
        series = 0;
    }

}

QQmlListProperty<SeriesData> SearchListModel::getSearchModel() {

    return QQmlListProperty<SeriesData>(this,&mySearchListModel,&SearchListModel::searchListCount,&SearchListModel::searchListAt);

}

// List handling methods

void SearchListModel::searchListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val)
{
    SearchListModel* searchModel = qobject_cast<SearchListModel*>(prop->object);
    searchModel->mySearchListModel.append(val);
}

SeriesData* SearchListModel::searchListAt(QQmlListProperty<SeriesData>* prop, int index)
{
    return (qobject_cast<SearchListModel*>(prop->object))->mySearchListModel.at(index);
}

int SearchListModel::searchListCount(QQmlListProperty<SeriesData>* prop)
{
    return qobject_cast<SearchListModel*>(prop->object)->mySearchListModel.size();
}

void SearchListModel::searchListClear(QQmlListProperty<SeriesData>* prop)
{
    qobject_cast<SearchListModel*>(prop->object)->mySearchListModel.clear();
}

void SearchListModel::xmlParseFinished() {

    qDebug("slotti toimii");
    mySeries = myReader->getSeries();
    populateSearchModel();
}

void SearchListModel::getFullSeriesRecordFinished() {

    storeSeries();
    storeEpisodes();
}

// -------------------------------------------------------------------
// POPULATING DATA

void SearchListModel::populateSearchModel() {


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


            mySearchListModel.append(serie);

        }


        // must remember to call signal to let QML side know about populated items..
        emit searchModelChanged();


    }
}

void SearchListModel::searchSeries(QString text) {

    mySearchListModel.clear();
    mySeries.clear();
    emit searchModelChanged();
    myReader->searchSeries(text);
}

void SearchListModel::selectSeries(int index) {

    myInfo = mySearchListModel.at(index);
    qDebug() << "myInfo seriesname: " << myInfo->getSeriesName();

}

void SearchListModel::getFullSeriesRecord(QString id) {
    myReader->getFullSeriesRecord(id);
    setLoading(true);

}

void SearchListModel::storeSeries() {

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

void SearchListModel::storeEpisodes() {

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

QString SearchListModel::getID() { return myInfo->getID(); }

QString SearchListModel::getLanguage() { return myInfo->getLanguage(); }

QString SearchListModel::getSeriesName() { return myInfo->getSeriesName(); }

QString SearchListModel::getAliasNames() { return myInfo->getAliasNames(); }

QString SearchListModel::getBanner() { return myInfo->getBanner(); }

QString SearchListModel::getOverview() { return myInfo->getOverview(); }

QString SearchListModel::getFirstAired() { return myInfo->getFirstAired(); }

QString SearchListModel::getIMDB_ID() { return myInfo->getIMDB_ID(); }

QString SearchListModel::getZap2it_ID() { return myInfo->getZap2it_ID(); }

QString SearchListModel::getNetwork() { return myInfo->getNetwork(); }

bool SearchListModel::getLoading() { return myLoading; }

void SearchListModel::setLoading(bool) {

    if(myLoading) {
        myLoading = false;
    } else {
        myLoading = true;
    }
    emit loadingChanged();
}

bool SearchListModel::getAdded() { return myAdded; }

void SearchListModel::setAdded(bool cond) {

    if(myAdded != cond) {
        myAdded = cond;
        emit addedChanged();
    }

}

void SearchListModel::clearList() {
    mySearchListModel.clear();
    emit searchModelChanged();
}

void SearchListModel::checkIfAdded(QString id, QString name) {

    if(mydbmanager->isAlreadyAdded(id.toInt(),name)) {
        setAdded(true);
        qDebug() << "already added";
    } else {
        setAdded(false);
        qDebug() << "not added";
    }
}


