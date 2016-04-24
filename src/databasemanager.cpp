#include "databasemanager.h"
#include "qcoreapplication.h"

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{
}

DatabaseManager::~DatabaseManager() {

    qDebug() << "destructing dbmanager";
    close();

}

void DatabaseManager::setUpDB() {

    if(openDB()) {

        // check whether db is empty, if it is, create db
        if(db.tables().size() == 0) {

            createDB();

        } else {

            QSqlQuery query(db);
            query.exec("SELECT name,version FROM information;");
            if(query.isSelect()) {
                while(query.next()) {
                    QString name = query.value(0).toString();
                    double version = query.value(1).toDouble();
                    qDebug() << "App name: " << name;
                    qDebug() << "Database version : " << version;

                    if(version == 1.0) {
                        qDebug() << "Needs first update. Add the Banner table to db";
                        if(createBannerTable()) {
                            updateInfoTable(2.0);
                        }
                    }
                }
            }
        }
    }
}

bool DatabaseManager::openDB() {

    QString dbname = "sailSeries.db.sqlite";
    QString dbpath = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + dbname;
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));

    if(!dir.exists()) {
        dir.mkpath(".");
    }

    // Find SQLite driver
    db = QSqlDatabase::addDatabase("QSQLITE","databasemanager");
    db.setDatabaseName(dbpath);

    // Open databasee
    return db.open();
}

QSqlError DatabaseManager::lastError() {

    // If opening database has failed user can ask
    // error description by QSqlError::text()
    return db.lastError();
}

void DatabaseManager::close() {

    db.close();

}

bool DatabaseManager::deleteDB() {

    // Close database
    db.close();

    // Remove created database binary file
    QString dbname = "sailSeries.db.sqlite";
    QString dbpath = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + dbname;
    return QFile::remove(dbpath);
}

bool DatabaseManager::startTransaction() {
    return db.transaction();
}

bool DatabaseManager::commit() {
    return db.commit();
}

bool DatabaseManager::initializeInfoTable() {

    bool ret = createInfoTable();

    if(ret) {

        QSqlQuery query(db);
        return query.exec(QString("INSERT INTO information VALUES(%1,'%2')")
                          .arg(1.0).arg("SailSeries"));
    }
    return false;
}

bool DatabaseManager::createInfoTable() {

    bool ret = false;

    if(db.isOpen()) {

        QSqlQuery query(db);
        ret = query.exec("CREATE TABLE information"
                         "(version real primary key, "
                         "name varchar(50))");
    }
    return ret;
}

bool DatabaseManager::updateInfoTable(double version) {

    bool ret = false;
    if(db.isOpen()) {

        QSqlQuery query(db);
        ret = query.exec(QString("UPDATE information SET version = %1;").arg(version));
    }
    return ret;
}

bool DatabaseManager::createDB() {

    qDebug() << "creating database!";

    if(db.isOpen()) {

        if(initializeInfoTable()) {
            qDebug() << "information table created";
        }

        if(createSeriesTable() and createEpisodeTable() and createBannerTable()) {

            qDebug() << "Tables created";

            updateInfoTable(2.0);
        }
    }

    return true;
}

bool DatabaseManager::createSeriesTable() {

    bool ret = false;
    if (db.isOpen()) {

        QSqlQuery query(db);
        ret = query.exec(QString("CREATE TABLE Series"
                                 "(id INTEGER PRIMARY KEY, "
                                 "actors VARCHAR(1000), "
                                 "airsDayOfWeek VARCHAR(15), "
                                 "airsTime VARCHAR(10), "
                                 "contentRating VARCHAR(5), "
                                 "firstAired VARCHAR(10), "
                                 "genre VARCHAR(100), "
                                 "imdbID VARCHAR(20), "
                                 "language VARCHAR(2), "
                                 "network VARCHAR(20), "
                                 "overview VARCHAR(1000), "
                                 "rating REAL, "
                                 "ratingCount INTEGER, "
                                 "runtime INTEGER, "
                                 "seriesName VARCHAR(20), "
                                 "status VARCHAR(20), "
                                 "added VARCHAR(20), "
                                 "addedBy INTEGER, "
                                 "banner VARCHAR(50), "
                                 "fanart VARCHAR(50), "
                                 "lastupdated VARCHAR(20), "
                                 "poster VARCHAR(50), "
                                 "zap2itID VARCHAR(20), "
                                 "watched INTEGER DEFAULT 0)"));

        //qDebug() << query.lastError();
    }
    return ret;
}

bool DatabaseManager::createEpisodeTable() {

    bool ret = false;
    if (db.isOpen()) {

        QSqlQuery query(db);
        ret = query.exec(QString("CREATE TABLE Episode"
                                 "(id INTEGER PRIMARY KEY, "
                                 "director VARCHAR(1000), "
                                 "epimgflag INTEGER, "
                                 "episodeName VARCHAR(50), "
                                 "episodeNumber INTEGER, "
                                 "firstAired VARCHAR(10), "
                                 "guestStars VARCHAR(1000), "
                                 "imdbID VARCHAR(20), "
                                 "language VARCHAR(20), "
                                 "overview VARCHAR(1000), "
                                 "productionCode INTEGER, "
                                 "rating REAL, "
                                 "ratingCount INTEGER, "
                                 "seasonNumber INTEGER, "
                                 "writer VARCHAR(1000), "
                                 "absoluteNumber INTEGER, "
                                 "airsAfterSeason INTEGER, "
                                 "airsBeforeEpisode INTEGER, "
                                 "airsBeforeSeason INTEGER, "
                                 "filename VARCHAR(50), "
                                 "lastupdated VARCHAR(20), "
                                 "seasonID INTEGER, "
                                 "seriesID INTEGER, "
                                 "thumbAdded VARCHAR(20), "
                                 "thumbHeight INTEGER, "
                                 "thumbWidth INTEGER, "
                                 "watched INTEGER DEFAULT 0)"));

        qDebug() << query.lastError();
        qDebug() << query.lastQuery();
    }
    return ret;
}

bool DatabaseManager::createBannerTable() {

    bool ret = false;
    if (db.isOpen()) {

        QSqlQuery query(db);
        ret = query.exec(QString("CREATE TABLE Banner"
                                 "(id INTEGER PRIMARY KEY, "
                                 "seriesID INTEGER, "
                                 "bannerPath VARCHAR(50), "
                                 "bannerType VARCHAR(50), "
                                 "bannerType2 VARCHAR(50), "
                                 "language VARCHAR(2), "
                                 "season INTEGER)"));

        qDebug() << query.lastError();
        qDebug() << query.lastQuery();
    }
    return ret;
}

bool DatabaseManager::insertSeries(QMap<QString, QString> series) {

    bool ret = false;
    int watched = 0;

    startTransaction();

    int seriesId = series["id"].toInt();
    QString actors = series["Actors"];
    QString airsDayOfWeek = series["Airs_DayOfWeek"];
    QString airsTime = series["Airs_Time"];
    QString contentRating = series["ContentRating"];
    QString firstAired = series["FirstAired"];
    QString genre = series["Genre"];
    QString imdb_id = series["IMDB_ID"];
    QString language = series["Language"];
    QString network = series["Network"];
    QString overview = series["Overview"];
    double rating = series["Rating"].toDouble();
    int ratingCount = series["RatingCount"].toInt();
    int runtime = series["Runtime"].toInt();
    QString seriesName = series["SeriesName"];
    QString status = series["Status"];
    QString added = series["added"];
    int addedby = series["addedBy"].toInt();
    QString banner = series["banner"];
    QString fanart = series["fanart"];
    QString lastUpdated = series["lastupdated"];
    QString poster = series["poster"];
    QString zap2itid = series["zap2it_id"];

    overview.replace("'","''");
    actors.replace("'","''");
    seriesName.replace("'","''");

    qDebug() << "insertSeries() " << seriesName;

    if(db.isOpen()) {

        QSqlQuery query(db);
        ret = query.exec(QString("INSERT OR REPLACE INTO Series(id,actors,airsDayOfWeek,airsTime,contentRating,firstAired,genre,imdbID,language,network,overview,rating,ratingCount,runtime,seriesName,status,added,addedBy,banner,fanart,lastupdated,poster,zap2itID,watched) VALUES(%1,'%2','%3','%4','%5','%6','%7','%8','%9','%10','%11',%12,%13,%14,'%15','%16','%17',%18,'%19','%20','%21','%22','%23',%24)")
                         .arg(seriesId)
                         .arg(actors)
                         .arg(airsDayOfWeek)
                         .arg(airsTime)
                         .arg(contentRating)
                         .arg(firstAired)
                         .arg(genre)
                         .arg(imdb_id)
                         .arg(language)
                         .arg(network)
                         .arg(overview)
                         .arg(rating)
                         .arg(ratingCount)
                         .arg(runtime)
                         .arg(seriesName)
                         .arg(status)
                         .arg(added)
                         .arg(addedby)
                         .arg(banner)
                         .arg(fanart)
                         .arg(lastUpdated)
                         .arg(poster)
                         .arg(zap2itid)
                         .arg(watched));

        qDebug() << query.lastError();

        if(query.lastError().text() != " ") {
            qDebug() << query.lastQuery();
        }

    }

    commit();

    return ret;

}

bool DatabaseManager::insertEpisodes(QList<QMap<QString, QString> > episodes) {

    bool ret = false;

    startTransaction();

    int length = episodes.length();
    for(int i = 0; i < length; ++i) {

        QMap<QString, QString> episode = episodes.at(i);

        int id = episode["id"].toInt();
        QString director = episode["Director"];
        int epimgflag = episode["EpImgFlag"].toInt();
        QString episodeName = episode["EpisodeName"];
        int episodeNumber = episode["EpisodeNumber"].toInt();
        QString firstAired = episode["FirstAired"];
        QString guestStars = episode["GuestStars"];
        QString imdb_id = episode["IMDB_ID"];
        QString language = episode["Language"];
        QString overview = episode["Overview"];
        int productionCode = episode["ProductionCode"].toInt();
        double rating = episode["Rating"].toDouble();
        int ratingCount = episode["RatingCount"].toInt();
        int seasonNumber = episode["SeasonNumber"].toInt();
        QString writer = episode["Writer"];
        int absoluteNumber = episode["absolute_number"].toInt();
        int airsAfterSeason = episode["airsafter_season"].toInt();
        int airsBeforeEpisode = episode["airsbefore_episode"].toInt();
        int airsBeforeSeason = episode["airsbefore_season"].toInt();
        QString filename = episode["filename"];
        QString lastUpdated = episode["lastupdated"];
        int seasonId = episode["seasonid"].toInt();
        int seriesId = episode["seriesid"].toInt();
        QString thumbAdded = episode["thumb_added"];
        int thumbHeight = episode["thumb_height"].toInt();
        int thumbWidth = episode["thumb_width"].toInt();
        int watched = 0;

        // important!
        overview.replace("'","''");
        director.replace("'","''");
        guestStars.replace("'","''");
        episodeName.replace("'","''");

        if(db.isOpen()) {

            // lets get and check the watched flag first, might be slow but this must be done
            // in order to keep the flag state
            QSqlQuery query(db);
            query.exec(QString("SELECT watched FROM Episode WHERE id = %1").arg(id));
            // qDebug() << query.lastError();

            if(query.isSelect()) {

                while(query.next()) {
                    watched = query.value(0).toInt();
                }
            }

            ret = query.exec(QString("INSERT OR REPLACE INTO Episode VALUES(%1,'%2',%3,'%4',%5,'%6','%7','%8','%9','%10',%11,%12,%13,%14,'%15',%16,%17,%18,%19,'%20','%21',%22,%23,'%24',%25,%26,%27)")
                             .arg(id)
                             .arg(director)
                             .arg(epimgflag)
                             .arg(episodeName)
                             .arg(episodeNumber)
                             .arg(firstAired)
                             .arg(guestStars)
                             .arg(imdb_id)
                             .arg(language)
                             .arg(overview)
                             .arg(productionCode)
                             .arg(rating)
                             .arg(ratingCount)
                             .arg(seasonNumber)
                             .arg(writer)
                             .arg(absoluteNumber)
                             .arg(airsAfterSeason)
                             .arg(airsBeforeEpisode)
                             .arg(airsBeforeSeason)
                             .arg(filename)
                             .arg(lastUpdated)
                             .arg(seasonId)
                             .arg(seriesId)
                             .arg(thumbAdded)
                             .arg(thumbHeight)
                             .arg(thumbWidth)
                             .arg(watched));

            qDebug() << query.lastError().text();
        }
        // process pending events to not freeze the app
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    }

    commit();

    return ret;

}

bool DatabaseManager::insertBanners(QList<QMap<QString, QString> > banners, int seriesId) {

    bool ret = false;

    startTransaction();

    int length = banners.size();
    for(int i = 0; i < length; ++i) {

        QMap<QString,QString> banner = banners.at(i);

        int id = banner["id"].toInt();
        QString bannerPath = banner["BannerPath"];
        QString bannerType = banner["BannerType"];
        QString bannerType2 = banner["BannerType2"];
        QString language = banner["Language"];
        int season = banner["Season"].toInt();

        if(db.isOpen()) {

            QSqlQuery query(db);
            ret = query.exec(QString("INSERT OR REPLACE INTO Banner VALUES(%1,%2,'%3','%4','%5','%6',%7)")
                             .arg(id)
                             .arg(seriesId)
                             .arg(bannerPath)
                             .arg(bannerType)
                             .arg(bannerType2)
                             .arg(language)
                             .arg(season));

            qDebug() << query.lastError();

        }

        // process pending events to not freeze the app
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    }

    commit();

    return ret;
}

QList<QMap<QString, QString> > DatabaseManager::getSeries() {

    QList<QMap<QString, QString> > allSeries;

    if(db.isOpen()) {

        this->startTransaction();
        QSqlQuery query(db);
        query.exec(QString("SELECT banner,poster,seriesName,status,id,overview,imdbID,rating FROM Series ORDER BY seriesName;"));
        this->commit();

        if(query.isSelect()) {
            while(query.next()) {

                QMap<QString, QString> temp;

                QString banner = query.value(0).toString();
                temp["banner"] = banner;

                QString poster = query.value(1).toString();
                temp["poster"] = poster;

                QString seriesName = query.value(2).toString();
                temp["seriesName"] = seriesName;

                QString status = query.value(3).toString();
                temp["status"] = status;

                int id = query.value(4).toInt();
                QString idstring = QString::number(id);
                temp["id"] = idstring;

                QString overview = query.value(5).toString();
                overview.replace("''","'");
                temp["overview"] = overview;

                QString imdbId = query.value(6).toString();
                temp["imdbId"] = imdbId;

                QString rating = query.value(7).toString();
                temp["rating"] = rating;

                int watched = watchedCount(id);
                QString watchedCount = QString::number(watched);
                temp["watchedCount"] = watchedCount;

                int total = totalCount(id);
                QString totalCount = QString::number(total);
                temp["totalCount"] = totalCount;

                allSeries.append(temp);

            }
        }
    }
    return allSeries;
}

QList<QMap<QString, QString> > DatabaseManager::getStartPageSeries() {

    auto date = QDateTime::currentDateTime().date();
    auto locale  = QLocale(QLocale::English);
    auto today = locale.toString(date, "dddd");
    auto firstAired = date.toString(Qt::ISODate);
    auto firstAiredStart = date.addDays(1 - date.dayOfWeek()).toString(Qt::ISODate);
    auto firstAiredEnd = date.addDays(7 - date.dayOfWeek()).toString(Qt::ISODate);
    auto status = "Continuing";

    QList<QMap<QString, QString> > series;

    if(db.isOpen()) {

        QSqlQuery query(db);
        query.exec(QString("SELECT Series.seriesName, Series.network, Series.airsTime, Series.airsDayOfWeek, Series.status, Series.id, Episode.episodeName, Episode.episodeNumber, Episode.seasonNumber, Episode.firstAired, Episode.filename, Episode.overview, Episode.guestStars, Episode.writer FROM Series, Episode WHERE Series.status = '%1' AND Episode.firstAired BETWEEN '%2' AND '%3' AND Series.id = Episode.seriesID AND Episode.seasonNumber != 0 ORDER BY Episode.firstAired;").arg(status).arg(firstAiredStart).arg(firstAiredEnd));

        if(query.isSelect()) {

            while(query.next()) {

                QMap<QString, QString> temp;

                auto seriesName = query.value(0).toString();
                temp["seriesName"] = seriesName;

                auto network = query.value(1).toString();
                temp["network"] = network;

                auto airsTime = query.value(2).toString();
                auto time = QTime::fromString(airsTime,"h:m A");
                airsTime = time.toString("h:mm");

                temp["airsTime"] = airsTime;

                auto airsDayOfWeek = query.value(3).toString();
                temp["airsDayOfWeek"] = airsDayOfWeek;

                auto status = query.value(4).toString();
                temp["status"] = status;

                auto id = query.value(5).toString();
                temp["id"] = id;

                auto episodeName = query.value(6).toString();
                temp["nextEpisodeName"] = episodeName;

                auto episodeNumber = query.value(7).toString();
                temp["nextEpisodeNumber"] = episodeNumber;

                auto seasonNumber = query.value(8).toString();
                temp["nextEpisodeSeasonNumber"] = seasonNumber;

                auto firstAired = query.value(9).toString();
                temp["nextEpisodeFirstAired"] = firstAired;

                auto banner = query.value(10).toString();
                temp["nextEpisodeBanner"] = banner;

                auto overview = query.value(11).toString();
                overview.replace("''","'");
                temp["nextEpisodeOverview"] = overview;

                auto guestStars = query.value(12).toString();
                temp["nextEpisodeGuestStars"] = guestStars;

                auto writer = query.value(13).toString();
                temp["nextEpisodeWriter"] = writer;

                series.append(temp);

            }
        }
    }
    return series;
}

QList<QMap<QString, QString> > DatabaseManager::getEpisodes(int seriesID, int seasonNumber) {

    QList<QMap<QString, QString> > episodes;

    QSqlQuery query(db);
    query.exec(QString("SELECT episodeName,episodeNumber,overview,seasonNumber,absoluteNumber,filename,watched,id,guestStars,writer,firstAired FROM Episode WHERE seriesID = %1 AND seasonNumber = %2 ORDER BY episodeNumber").arg(seriesID).arg(seasonNumber));

    if(query.isSelect()) {

        while(query.next()) {

            QMap<QString, QString> temp;

            QString episodeName = query.value(0).toString();
            temp["episodeName"] = episodeName;

            QString episodeNumber = query.value(1).toString();
            temp["episodeNumber"] = episodeNumber;

            QString overview = query.value(2).toString();
            overview.replace("''","'");
            temp["overview"] = overview;

            QString seasonNumber = query.value(3).toString();
            temp["seasonNumber"] = seasonNumber;

            QString absoluteNumber = query.value(4).toString();
            temp["absoluteNumber"] = absoluteNumber;

            QString filename = query.value(5).toString();
            temp["filename"] = filename;

            QString watched = query.value(6).toString();
            temp["watched"] = watched;

            QString id = query.value(7).toString();
            temp["id"] = id;

            QString guestStars = query.value(8).toString();
            temp["guestStars"] = guestStars;

            QString writer = query.value(9).toString();
            temp["writer"] = writer;

            QString firstAired = query.value(10).toString();
            temp["firstAired"] = firstAired;

            episodes.append(temp);
        }
    }
    return episodes;
}

void DatabaseManager::toggleWatched(QString episodeID) {

    QSqlQuery query(db);
    query.exec(QString("SELECT watched FROM Episode WHERE id = %1").arg(episodeID.toInt()));
    if(query.isSelect()) {
        while(query.next()) {
            if(query.value(0).toInt() == 0) {
                query.exec(QString("UPDATE Episode SET watched = 1 WHERE id = %1").arg(episodeID.toInt()));
                qDebug() << query.lastError();
            } else {
                query.exec(QString("UPDATE Episode SET watched = 0 WHERE id = %1").arg(episodeID.toInt()));
                qDebug() << query.lastError();
            }
        }
    }
}

bool DatabaseManager::deleteSeries(int seriesID) {

    bool ret1,ret2,ret3 = false;

    this->startTransaction();

    QSqlQuery query(db);
    ret1 = query.exec(QString("DELETE FROM Series WHERE id = %1").arg(seriesID));
    if(ret1) {
        ret2 = query.exec(QString("DELETE FROM Episode WHERE seriesID = %1").arg(seriesID));
        if(ret1 && ret2) {
            ret3 = query.exec(QString("DELETE FROM Banner WHERE seriesID = %1").arg(seriesID));
        }
    }

    this->commit();

    return ret1 && ret2 && ret3;
}

bool DatabaseManager::deleteAllSeries() {

    bool ret1,ret2,ret3 = false;

    this->startTransaction();

    QSqlQuery query(db);
    ret1 = query.exec(QString("DELETE FROM Series"));
    if(ret1) {
        ret2 = query.exec(QString("DELETE FROM Episode"));
        if(ret1 && ret2) {
            ret3 = query.exec(QString("DELETE FROM Banner"));
        }
    }

    this->commit();

    return ret1 && ret2 && ret3;
}

bool DatabaseManager::isAlreadyAdded(int seriesID,QString name) {

    bool ret1,ret2 = false;

    QSqlQuery query(db);
    ret1 = query.exec(QString("SELECT seriesName FROM Series WHERE id=%1").arg(seriesID));
    if(ret1) {
        if(query.isSelect()) {
            while(query.next()) {
                if(query.value(0).toString() == name) {
                    ret2 = true;
                }
            }
        }
    }
    return ret2;
}

int DatabaseManager::watchedCount(int seriesID) {

    int watchedCount = 0;
    QSqlQuery query(db);
    query.exec(QString("SELECT COUNT(episodeName) FROM Episode WHERE seriesID = %1 AND watched = 1 AND seasonNumber != 0").arg(seriesID));
    if(query.isSelect()) {
        while(query.next()) {
            watchedCount = query.value(0).toInt();
        }
    }
    return watchedCount;
}

int DatabaseManager::watchedCountBySeason(int seriesID, int seasonNumber) {

    int watchedCount = 0;
    QSqlQuery query(db);
    query.exec(QString("SELECT COUNT(episodeName) FROM Episode WHERE seriesID = %1 AND watched = 1 AND seasonNumber = %2").arg(seriesID).arg(seasonNumber));
    if(query.isSelect()) {
        while(query.next()) {
            watchedCount = query.value(0).toInt();
        }
    }
    return watchedCount;

}

int DatabaseManager::totalCount(int seriesID) {

    int totalCount = 0;
    QSqlQuery query(db);
    query.exec(QString("SELECT COUNT(episodeName) FROM Episode WHERE seriesID = %1 AND seasonNumber != 0").arg(seriesID));
    if(query.isSelect()) {
        while(query.next()) {
            totalCount = query.value(0).toInt();
        }
    }
    return totalCount;

}

int DatabaseManager::totalCountBySeason(int seriesID, int seasonNumber) {

    int totalCount = 0;
    QSqlQuery query(db);
    query.exec(QString("SELECT COUNT(episodeName) FROM Episode WHERE seriesID = %1 AND seasonNumber = %2").arg(seriesID).arg(seasonNumber));
    if(query.isSelect()) {
        while(query.next()) {
            totalCount = query.value(0).toInt();
        }
    }
    return totalCount;

}

int DatabaseManager::seasonCount(int seriesID) {

    int seasonCount = 0;
    QSqlQuery query(db);
    query.exec(QString("SELECT MAX(seasonNumber) FROM Episode WHERE seriesID = %1").arg(seriesID));
    if(query.isSelect()) {
        while(query.next()) {
            seasonCount = query.value(0).toInt();
        }
    }
    return seasonCount;
}

void DatabaseManager::markSeasonWatched(int seriesID, int season) {

    QSqlQuery query(db);
    query.exec(QString("UPDATE Episode SET watched=1 WHERE seriesID = %1 AND seasonNumber = %2").arg(seriesID).arg(season));
    qDebug() << query.lastError();

}

// Get the details of next episode
QMap<QString,QString> DatabaseManager::getNextEpisodeDetails(int seriesID) {

    QList<QMap<QString, QString> > details;

    this->startTransaction();
    QSqlQuery query(db);
    query.exec(QString("SELECT episodeName,episodeNumber,seasonNumber,firstAired FROM Episode WHERE seriesID = %1 AND seasonNumber != 0 ORDER BY absoluteNumber;").arg(seriesID));
    this->commit();

    if(query.isSelect()) {

        while(query.next()) {

            QMap<QString, QString> temp;

            QString episodeName = query.value(0).toString();
            temp["episodeName"] = episodeName;

            QString episodeNumber = query.value(1).toString();
            temp["episodeNumber"] = episodeNumber;

            QString seasonNumber = query.value(2).toString();
            temp["seasonNumber"] = seasonNumber;

            QString firstAired = query.value(3).toString();
            temp["firstAired"] = firstAired;

            details.append(temp);
        }
    }

    // find next firstAirday that is today or the next after today,
    // store "today" or "tomorrow" or just number or unknown

    QMap<QString,QString> nextEpisodeDetails;

    QDate date = QDateTime::currentDateTime().date();
    QString today = date.toString(Qt::ISODate);
    int daysTo = 10000;

    int size = details.size();
    for(int i = 0; i < size; ++i) {

        QMap<QString, QString> temp = details.at(i);
        QString firstAired = temp["firstAired"];
        int daysToNext = QDate::fromString(today,"yyyy-MM-dd").daysTo(QDate::fromString(firstAired,"yyyy-MM-dd"));

        if(daysToNext >= 0 && daysToNext < daysTo && firstAired.size() != 0) {
            daysTo = daysToNext;
            nextEpisodeDetails = temp;
        }
    }

    if(daysTo == 0) {
        nextEpisodeDetails["daysToNext"] = "today";
    } else if(daysTo == 1) {
        nextEpisodeDetails["daysToNext"] = "tomorrow";
    } else if(daysTo == 10000){
        nextEpisodeDetails["daysToNext"] = "unknown";
    } else {
        nextEpisodeDetails["daysToNext"] = QString::number(daysTo);
    }

    return nextEpisodeDetails;
}

QString DatabaseManager::getStatus(int seriesID) {

    QSqlQuery query(db);
    QString status;
    query.exec(QString("SELECT status FROM Series WHERE id=%1;").arg(seriesID));
    if(query.isSelect()) {
        while(query.next()) {
            status = query.value(0).toString();
        }
    }
    return status;
}

QString DatabaseManager::getSeasonBanner(int seriesID, int season) {

    QSqlQuery query(db);
    QString banner = "";
    QString bannerType = "season";
    query.exec(QString("SELECT bannerPath FROM Banner WHERE seriesID=%1 AND bannerType='%2' AND season=%3;").arg(seriesID).arg(bannerType).arg(season));
    if(query.isSelect()) {
        while(query.next()) {
            banner = query.value(0).toString();
        }
    }
    return banner;
}
