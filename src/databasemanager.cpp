#include "databasemanager.h"

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{
}

DatabaseManager::~DatabaseManager() {

    close();

}

void DatabaseManager::setUpDB() {

    if(openDB()) {

        // check whether db is empty
        if(db.tables().size() == 0) {

            createDB();

        } else {

            QSqlQuery query(db);
            query.exec("SELECT version FROM information;");
            double version = query.value(0).toDouble();

            qDebug() << "Database already created, current version is: " << version;

            updateDB();
        }
    }
}

bool DatabaseManager::openDB() {

    QString dbname = "sailSeries.db.sqlite";
    QString dbpath = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + dbname;
    QString dirpath = QStandardPaths::writableLocation(QStandardPaths::DataLocation);
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

        //-----------------------------------------------------------------

        if(initializeInfoTable()) qDebug() << "information table created";

        //-----------------------------------------------------------------

        if(createSeriesTable() and createEpisodeTable()) {

            qDebug() << "Tables created";
        }
    }

    return true;
}

bool DatabaseManager::updateDB() {

    qDebug() << "päivitetään tietokantaa :)";
    return true;
}

bool DatabaseManager::createSeriesTable() {

    bool ret = false;
    if (db.isOpen()) {

        QSqlQuery query(db);
        ret = query.exec(QString("CREATE TABLE Series"
                                 "(id INTEGER , "
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

        qDebug() << query.lastError();
    }
    return ret;
}

bool DatabaseManager::createEpisodeTable() {

    bool ret = false;
    if (db.isOpen()) {

        QSqlQuery query(db);
        ret = query.exec(QString("CREATE TABLE Episode"
                                 "(id INTEGER, "
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

bool DatabaseManager::insertSeries(int id, QString actors, QString airsDayOfWeek, QString airsTime,
                                   QString contentRating, QString firstAired, QString genre, QString imdb_id,
                                   QString language, QString network, QString overview, double rating,
                                   int ratingCount, int runtime, QString SeriesName, QString status,
                                   QString added, int addedby, QString banner, QString fanart,
                                   QString lastUpdated, QString poster, QString zap2itid, int watched) {

    bool ret = false;

    overview.replace("'","''");
    actors.replace("'","''");
    SeriesName.replace("'","''");

    qDebug() << id;
    qDebug() << actors;
    qDebug() << airsDayOfWeek;
    qDebug() << airsTime;
    qDebug() << contentRating;
    qDebug() << firstAired;
    qDebug() << genre;
    qDebug() << imdb_id;
    qDebug() << language;
    qDebug() << network;
    qDebug() << overview;
    qDebug() << rating;
    qDebug() << ratingCount;
    qDebug() << runtime;
    qDebug() << SeriesName;
    qDebug() << status;
    qDebug() << added;
    qDebug() << addedby;
    qDebug() << banner;
    qDebug() << fanart;
    qDebug() << lastUpdated;
    qDebug() << poster;
    qDebug() << zap2itid;

    qDebug() << "insertSeries() " << SeriesName;

    if(db.isOpen()) {

        QSqlQuery query(db);
        ret = query.exec(QString("INSERT INTO Series(id,actors,airsDayOfWeek,airsTime,contentRating,firstAired,genre,imdbID,language,network,overview,rating,ratingCount,runtime,seriesName,status,added,addedBy,banner,fanart,lastupdated,poster,zap2itID,watched) VALUES(%1,'%2','%3','%4','%5','%6','%7','%8','%9','%10','%11',%12,%13,%14,'%15','%16','%17',%18,'%19','%20','%21','%22','%23',%24)")
                         .arg(id)
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
                         .arg(SeriesName)
                         .arg(status)
                         .arg(added)
                         .arg(addedby)
                         .arg(banner)
                         .arg(fanart)
                         .arg(lastUpdated)
                         .arg(poster)
                         .arg(zap2itid)
                         .arg(watched));

        qDebug() << query.lastError().text();

        if(query.lastError().text() != " ") {
            qDebug() << query.lastQuery();
        }

    }

    return ret;


    //    QByteArray bArray;
    //    QBuffer buffer( &bArray );
    //    buffer.open( QIODevice::WriteOnly );
    //    QSqlQuery query(db);

    //    banner.save( &buffer, "PNG" );
    //    query.prepare("INSERT INTO series (banner) VALUES (?)");
    //    query.addBindValue(bArray);
    //    query.exec();

    //    fanart.save( &buffer, "PNG" );
    //    query.prepare("INSERT INTO series (fanart) VALUES (?)");
    //    query.addBindValue(bArray);
    //    query.exec();

}

bool DatabaseManager::insertEpisode(int id, QString director, int epimgflag, QString episodeName,
                                    int episodeNumber, QString firstAired, QString guestStars, QString imdb_id,
                                    QString language, QString overview, int productionCode, double rating,
                                    int ratingCount, int seasonNumber, QString writer, int absoluteNumber,
                                    int airsAfterSeason, int airsBeforeEpisode, int airsBeforeSeason , QString filename,
                                    QString lastUpdated, int seasonID, int seriesID, QString thumbAdded,
                                    int thumbHeight, int thumbWidth, int watched) {

    bool ret = false;

    // important!
    overview.replace("'","''");
    director.replace("'","''");
    guestStars.replace("'","''");
    episodeName.replace("'","''");

    if(db.isOpen()) {

        QSqlQuery query(db);
        ret = query.exec(QString("INSERT INTO Episode VALUES(%1,'%2',%3,'%4',%5,'%6','%7','%8','%9','%10',%11,%12,%13,%14,'%15',%16,%17,%18,%19,'%20','%21',%22,%23,'%24',%25,%26,%27)")
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
                         .arg(seasonID)
                         .arg(seriesID)
                         .arg(thumbAdded)
                         .arg(thumbHeight)
                         .arg(thumbWidth)
                         .arg(watched));

        qDebug() << query.lastError().text();
        if(query.lastError().text() != " ") {
            qDebug() << query.lastQuery();
        }
    }

    return ret;

}

QList<QList<QString> > DatabaseManager::getSeries() {

    QList<QList<QString> > allSeries;

    if(db.isOpen()) {

        QSqlQuery query(db);
        query.exec(QString("SELECT banner,poster,seriesName,status,id,overview,imdbID,rating FROM Series ORDER BY seriesName;"));
        qDebug() << query.lastError();

        if(query.isSelect()) {
            while(query.next()) {

                QList<QString> temp;

                QString banner = query.value(0).toString();
                temp.append(banner);
                qDebug() << banner;

                QString poster = query.value(1).toString();
                temp.append(poster);
                qDebug() << poster;

                QString seriesName = query.value(2).toString();
                temp.append(seriesName);
                qDebug() << seriesName;

                QString status = query.value(3).toString();
                temp.append(status);
                qDebug() << status;

                int id = query.value(4).toInt();
                QString idstring = QString::number(id);
                temp.append(idstring);
                qDebug() << id;

                QString overview = query.value(5).toString();
                overview.replace("''","'");
                temp.append(overview);

                QString imdbID = query.value(6).toString();
                temp.append(imdbID);

                QString rating = query.value(7).toString();
                temp.append(rating);

                allSeries.append(temp);

            }


            return allSeries;
        }

    }
}

QList<QList<QString> > DatabaseManager::getStartPageSeries() {

    QList<QList<QString> > allSeries;

    if(db.isOpen()) {

        QSqlQuery query(db);
        query.exec(QString("SELECT seriesName,network,airsTime,airsDayOfWeek,status FROM Series ORDER BY seriesName;"));
        qDebug() << query.lastError();

        if(query.isSelect()) {
            while(query.next()) {

                QList<QString> temp;

                QString seriesName = query.value(0).toString();
                temp.append(seriesName);
                qDebug() << seriesName;

                QString network = query.value(1).toString();
                temp.append(network);
                qDebug() << network;

                QString airsTime = query.value(2).toString();
                temp.append(airsTime);
                qDebug() << airsTime;

                QString airsDayOfWeek = query.value(3).toString();
                temp.append(airsDayOfWeek);
                qDebug() << airsDayOfWeek;

                QString status = query.value(4).toString();
                temp.append(status);
                qDebug() << status;

                allSeries.append(temp);

            }

            return allSeries;
        }

    }
}

QList<QList<QString> > DatabaseManager::getEpisodes(int seriesID) {

    QList<QList<QString> > episodes;

    QSqlQuery query(db);
    query.exec(QString("SELECT episodeName,episodeNumber,overview,seasonNumber,absoluteNumber,filename,watched,id FROM Episode WHERE seriesID = %1 ORDER BY absoluteNumber").arg(seriesID));

    qDebug() << query.lastError();

    if(query.isSelect()) {
        while(query.next()) {

            QList<QString> temp;

            QString episodeName = query.value(0).toString();
            temp.append(episodeName);
            qDebug() << episodeName;

            QString episodeNumber = query.value(1).toString();
            temp.append(episodeNumber);

            QString overview = query.value(2).toString();
            overview.replace("''","'");
            temp.append(overview);

            QString seasonNumber = query.value(3).toString();
            temp.append(seasonNumber);

            QString absoluteNumber = query.value(4).toString();
            temp.append(absoluteNumber);

            QString filename = query.value(5).toString();
            temp.append(filename);

            QString watched = query.value(6).toString();
            temp.append(watched);

            QString id = query.value(7).toString();
            temp.append(id);

            episodes.append(temp);

            qDebug() << query.lastError();

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
            } else {
                query.exec(QString("UPDATE Episode SET watched = 0 WHERE id = %1").arg(episodeID.toInt()));
            }
        }
    }

}

bool DatabaseManager::deleteSeries(int seriesID) {

    bool ret1,ret2 = false;

    QSqlQuery query(db);
    ret1 = query.exec(QString("DELETE FROM Series WHERE id = %1").arg(seriesID));
    qDebug() << query.lastError();
    if(ret1) {
        ret2 = query.exec(QString("DELETE FROM Episode WHERE seriesID = %1").arg(seriesID));
        qDebug() << query.lastError();
    }

    db.commit();

    return ret2;
}

bool DatabaseManager::deleteAllSeries() {

    bool ret1,ret2 = false;

    QSqlQuery query(db);
    ret1 = query.exec(QString("DELETE FROM Series"));
    if(ret1) {
        ret2 = query.exec(QString("DELETE FROM Episode"));
    }

    db.commit();

    return ret2;
}

bool DatabaseManager::isAlreadyAdded(int seriesID,QString name) {

    bool ret1,ret2 = false;

    QSqlQuery query(db);
    ret1 = query.exec(QString("SELECT seriesName FROM Series WHERE id=%1").arg(seriesID));
    if(ret1) {
        if(query.isSelect()) {
            while(query.next()) {
                if(query.value(0).toString() == name) {
                    qDebug() << "Series found " << query.value(0).toString() << " == " << name;
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

int DatabaseManager::seasonCount(int seriesID) {

    int seasonCount = 0;
    QSqlQuery query(db);
    query.exec(QString("SELECT MAX(seasonNumber) FROM Episode WHERE seriesID = %1").arg(seriesID));
    if(query.isSelect()) {
        while(query.next()) {
            seasonCount = query.value(0).toInt();
            qDebug() << "number of seasons " << seasonCount;
        }
    }
    return seasonCount;
}

void DatabaseManager::markSeasonWatched(int seriesID, int season) {

    QSqlQuery query(db);
    query.exec(QString("UPDATE Episode SET watched=1 WHERE seriesID = %1 AND seasonNumber = %2").arg(seriesID).arg(season));
    qDebug() << query.lastError();

}

QList<QString> DatabaseManager::getNextEpisodeDetails(int seriesID) {

    QList<QList<QString> > details;

    QSqlQuery query(db);
    query.exec(QString("SELECT episodeName,episodeNumber,seasonNumber,firstAired FROM Episode WHERE seriesID = %1 ORDER BY absoluteNumber;").arg(seriesID));
    qDebug() << query.lastError();

    if(query.isSelect()) {
        while(query.next()) {

            QList<QString> temp;

            QString episodeName = query.value(0).toString();
            temp.append(episodeName);
            qDebug() << banner;

            QString episodeNumber = query.value(1).toString();
            temp.append(episodeNumber);
            qDebug() << poster;

            QString seasonNumber = query.value(2).toString();
            temp.append(seasonNumber);
            qDebug() << seriesName;

            QString firstAired = query.value(3).toString();
            temp.append(firstAired);
            qDebug() << status;

            details.append(temp);
        }

    }

    // Now we find out the actual next episode.

    QDate date = QDateTime::currentDateTime().date();
    QString ISODate = date.toString(Qt::ISODate);
    qDebug() << ISODate;

    int daysToNow;
    int smallest = 1000;

    for(int i = 0; i < details.size(); ++i) {

        QList<QString> temp = details.at(i);

        daysToNow = QDate::fromString(ISODate).daysTo(QDate::fromString(temp.at(3)));
        qDebug() << daysToNow;

        if(daysToNow > 0) {

            if(daysToNow < smallest) {
                smallest = daysToNow;
                qDebug() << smallest;
            }
        }
    }
}
