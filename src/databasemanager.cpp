#include "databasemanager.h"
#include "qcoreapplication.h"

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent) {}

DatabaseManager::~DatabaseManager()
{
    qDebug() << "destructing dbmanager";
    close();   
}

void DatabaseManager::setUpDB()
{
    if (openDB()) {
        
        // check whether db is empty, if it is, create db
        if (m_db.tables().size() == 0) {
            
            createDB();
            
        } else {
            
            QSqlQuery query(m_db);
            query.exec("SELECT name,version FROM information;");
            if (query.isSelect()) {
                while (query.next()) {
                    QString name = query.value(0).toString();
                    double version = query.value(1).toDouble();
                    qDebug() << "App name: " << name;
                    qDebug() << "Database version : " << version;
                    
                    if (version == 1.0) {
                        qDebug() << "Needs first update. Add the Banner table to db";
                        if (createBannerTable()) {
                            updateInfoTable(2.0);
                        }
                    }
                }
            }
        }
    }
}

bool DatabaseManager::openDB()
{
    QString dbname = "sailSeries.db.sqlite";
    QString dbpath = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + dbname;
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    // Find SQLite driver
    m_db = QSqlDatabase::addDatabase("QSQLITE","databasemanager");
    m_db.setDatabaseName(dbpath);
    
    // Open databasee
    return m_db.open();
}

QSqlError DatabaseManager::lastError()
{
    return m_db.lastError();
}

void DatabaseManager::close()
{
    m_db.close();   
}

bool DatabaseManager::deleteDB()
{
    // Close database
    m_db.close();
    
    // Remove created database binary file
    QString dbname = "sailSeries.db.sqlite";
    QString dbpath = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + dbname;
    return QFile::remove(dbpath);
}

bool DatabaseManager::startTransaction()
{
    return m_db.transaction();
}

bool DatabaseManager::commit()
{
    return m_db.commit();
}

bool DatabaseManager::initializeInfoTable()
{
    bool ret = createInfoTable();
    
    if (ret) {
        
        QSqlQuery query(m_db);
        return query.exec(QString("INSERT INTO information VALUES(%1,'%2')")
                          .arg(1.0).arg("SailSeries"));
    }
    return false;
}

bool DatabaseManager::createInfoTable()
{
    bool ret = false;
    
    if (m_db.isOpen()) {
        
        QSqlQuery query(m_db);
        ret = query.exec("CREATE TABLE information"
                         "(version real primary key, "
                         "name varchar(50))");
    }
    return ret;
}

bool DatabaseManager::updateInfoTable(double version)
{
    bool ret = false;
    if (m_db.isOpen()) {
        
        QSqlQuery query(m_db);
        ret = query.exec(QString("UPDATE information SET version = %1;").arg(version));
    }
    return ret;
}

bool DatabaseManager::createDB()
{
    qDebug() << "creating database!";
    
    if (m_db.isOpen()) {
        
        if (initializeInfoTable()) {
            qDebug() << "information table created";
        }
        
        if (createSeriesTable() and createEpisodeTable() and createBannerTable()) {
            
            qDebug() << "Tables created";
            
            updateInfoTable(2.0);
        }
    }
    return true;
}

bool DatabaseManager::createSeriesTable()
{
    bool ret = false;
    if (m_db.isOpen()) {
        
        QSqlQuery query(m_db);
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
    }
    return ret;
}

bool DatabaseManager::createEpisodeTable()
{
    bool ret = false;
    if (m_db.isOpen()) {
        
        QSqlQuery query(m_db);
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

bool DatabaseManager::createBannerTable()
{
    bool ret = false;
    if (m_db.isOpen()) {
        
        QSqlQuery query(m_db);
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

bool DatabaseManager::deleteDuplicateEpisodes()
{
    bool ret = false;
    if (m_db.isOpen()) {

        QSqlQuery query(m_db);
        ret = query.exec("DELETE FROM episode "
                         "WHERE (firstAired = '' OR firstAired IS NULL) AND (episodeName IS NULL OR episodeName = '');");

        qDebug() << query.lastError();
        qDebug() << query.lastQuery();
    }
    return ret;
}

bool DatabaseManager::insertSeries(QVariantMap series)
{
    bool ret = false;
    int watched = 0;
    
    startTransaction();
    
    int seriesId = series["id"].toInt();
    QString actors = series["actors"].toString();
    QString airsDayOfWeek = series["airsDayOfWeek"].toString();
    QString airsTime = series["airsTime"].toString();
    QString contentRating = series["contentRating"].toString();
    QString firstAired = series["firstAired"].toString();
    QString genre = series["genre"].toStringList().join(",");
    QString imdb_id = series["imdbId"].toString();
    QString language = series["language"].toString();
    QString network = series["network"].toString();
    QString overview = series["overview"].toString();
    double rating = series["siteRating"].toDouble();
    int ratingCount = series["siteRatingCount"].toInt();
    int runtime = series["runtime"].toInt();
    QString seriesName = series["seriesName"].toString();
    QString status = series["status"].toString();
    QString added = series["added"].toString();
    int addedby = series["addedBy"].toInt();
    QString banner = series["banner"].toString();
    QString fanart = series["fanart"].toString();
    QString lastUpdated = series["lastUpdated"].toString();
    QString poster = series["poster"].toString();
    QString zap2itid = series["zap2itId"].toString();
    
    overview.replace("'", "''");
    actors.replace("'", "''");
    seriesName.replace("'", "''");
    
    qDebug() << "insertSeries() " << seriesName;
    
    if (m_db.isOpen()) {
        
        QSqlQuery query(m_db);
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
        
        if (query.lastError().text() != " ") {
            qDebug() << query.lastQuery();
        }
    }
    
    commit();
    
    return ret;
}

bool DatabaseManager::insertEpisodes(QList<QVariantMap> episodes, int seriesId) {

    qDebug() << "insertEpisodes() ";
    
    bool ret = false;
    
    startTransaction();

    for (auto episode : episodes) {
        
        int absoluteNumber = episode["absoluteNumber"].toInt();
        int episodeNumber = episode["airedEpisodeNumber"].toInt();
        int seasonNumber = episode["airedSeason"].toInt();
        int seasonId = episode["airedSeasonID"].toInt();
        QString episodeName = episode["episodeName"].toString();
        QString firstAired = episode["firstAired"].toString();
        int id = episode["id"].toInt();
        QString language = episode["language"].toMap()["overview"].toString();
        QString lastUpdated = episode["lastUpdated"].toString();
        QString overview = episode["overview"].toString();
        int watched = 0;

        // OLD API PROVIDED THESE
        /*QString director = episode["director"].toString();
        int epimgflag = episode["EpImgFlag"].toInt();
        QString guestStars = episode["GuestStars"].toString();
        QString imdb_id = episode["IMDB_ID"].toString();
        int productionCode = episode["ProductionCode"].toInt();
        double rating = episode["Rating"].toDouble();
        int ratingCount = episode["RatingCount"].toInt();
        QString writer = episode["Writer"].toString();
        int airsAfterSeason = episode["airsafter_season"].toInt();
        int airsBeforeEpisode = episode["airsbefore_episode"].toInt();
        int airsBeforeSeason = episode["airsbefore_season"].toInt();
        QString filename = episode["filename"].toString();
        QString thumbAdded = episode["thumb_added"].toString();
        int thumbHeight = episode["thumb_height"].toInt();
        int thumbWidth = episode["thumb_width"].toInt();*/

        // important!
        overview.replace("'", "''");
//        director.replace("'", "''");
//        guestStars.replace("'", "''");
        episodeName.replace("'", "''");
        
        if (m_db.isOpen()) {

            // lets get and check the watched flag first, might be slow but this must be done
            // in order to keep the flag state
            QSqlQuery query(m_db);
            query.prepare("SELECT watched FROM Episode WHERE id = :id");
            query.bindValue(":id", id);
            query.exec();
            
            if (query.isSelect()) {
                
                while (query.next()) {
                    watched = query.value(0).toInt();
                }
            }
            
            query.prepare("INSERT OR REPLACE INTO Episode (id, episodeName, episodeNumber, firstAired, language, overview, seasonNumber, absoluteNumber, lastupdated, seasonID, seriesID, watched) "
                          "VALUES (:id, :episodeName, :episodeNumber, :firstAired, :language, :overview, :seasonNumber, :absoluteNumber, :lastUpdated, :seasonId, :seriesId, :watched)");
            query.bindValue(":id", id);
            query.bindValue(":episodeName", episodeName);
            query.bindValue(":episodeNumber", episodeNumber);
            query.bindValue(":firstAired", firstAired);
            query.bindValue(":language", language);
            query.bindValue(":overview", overview);
            query.bindValue(":seasonNumber", seasonNumber);
            query.bindValue(":absoluteNumber", absoluteNumber);
            query.bindValue(":lastUpdated", lastUpdated);
            query.bindValue(":seasonId", seasonId);
            query.bindValue(":seriesId", seriesId);
            query.bindValue(":watched", watched);
            ret = query.exec();
        }

        // process pending events to not freeze the app
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    }
    
    commit();
    
    return ret;
}

bool DatabaseManager::insertBanners(QList<QVariantMap> banners, int seriesId)
{
    qDebug() << "insertBanners() ";
    
    bool ret = false;
    
    startTransaction();

    for (auto banner : banners) {

        auto id = banner["id"].toInt();
        auto bannerPath = banner["bannerPath"].toString();
        auto bannerType = banner["bannerType"].toString();
        auto bannerType2 = banner["bannerType2"].toString();
        auto language = banner["language"].toString();
        auto season = banner["season"].toInt();
        
        if (m_db.isOpen()) {
            
            QSqlQuery query(m_db);
            query.prepare("INSERT OR REPLACE INTO Banner (id, seriesID, bannerPath, bannerType, bannerType2, language, season) "
                          "VALUES (:id, :seriesId, :bannerPath, :bannerType, :bannerType2, :language, :season)");
            query.bindValue(":id", id);
            query.bindValue(":seriesId", seriesId);
            query.bindValue(":bannerPath", bannerPath);
            query.bindValue(":bannerType", bannerType);
            query.bindValue(":bannerType2", bannerType2);
            query.bindValue(":language", language);
            query.bindValue(":season", season);
            ret = query.exec();
        }
        
        // process pending events to not freeze the app
        qApp->processEvents(QEventLoop::ExcludeUserInputEvents);
    }
    
    commit();
    
    return ret;
}

QList<QVariantMap> DatabaseManager::getSeries()
{
    QList<QVariantMap> allSeries;
    
    if (m_db.isOpen()) {
        
        this->startTransaction();

        QSqlQuery query(m_db);
        query.exec(QString("SELECT banner, poster, seriesName, status, id, overview, imdbID, rating, genre "
                           "FROM Series "
                           "ORDER BY seriesName;"));
        this->commit();
        
        if (query.isSelect()) {

            while (query.next()) {
                
                QVariantMap temp;
                
                auto banner = query.value(0).toString();
                temp["banner"] = banner;
                
                auto poster = query.value(1).toString();
                temp["poster"] = poster;
                
                auto seriesName = query.value(2).toString();
                temp["seriesName"] = seriesName;
                
                auto status = query.value(3).toString();
                temp["status"] = status;
                
                auto id = query.value(4).toInt();
                auto idString = QString::number(id);
                temp["id"] = idString;
                
                auto overview = query.value(5).toString();
                overview.replace("''", "'");
                temp["overview"] = overview;
                
                auto imdbId = query.value(6).toString();
                temp["imdbId"] = imdbId;
                
                auto rating = query.value(7).toString();
                temp["rating"] = rating;
                
                auto genre = query.value(8).toString();
                temp["genre"] = genre;
                
                auto watched = watchedCount(id);
                auto watchedCount = QString::number(watched);
                temp["watchedCount"] = watchedCount;
                
                auto total = totalCount(id);
                auto totalCount = QString::number(total);
                temp["totalCount"] = totalCount;
                
                allSeries.append(temp);
            }
        }
    }
    return allSeries;
}

QList<QVariantMap> DatabaseManager::getStartPageSeries()
{
    auto date = QDateTime::currentDateTime().date();
    auto locale  = QLocale(QLocale::English);
    auto firstAiredStart = date.addDays(1 - date.dayOfWeek()).toString(Qt::ISODate); // Monday == 1
    auto firstAiredEnd = date.addDays(7 - date.dayOfWeek()).toString(Qt::ISODate); // Sunday == 7
    auto status = "Continuing";
    const auto kTwelveHoursInSecs = 12 * 60 * 60;
    
    QList<QVariantMap> series;
    
    if (m_db.isOpen()) {
        
        QSqlQuery query(m_db);
        query.exec(QString("SELECT Series.seriesName, Series.network, Series.airsTime, Series.airsDayOfWeek, Series.status, Series.id, Episode.id, Episode.episodeName, Episode.episodeNumber, Episode.seasonNumber, Episode.firstAired, Episode.filename, Episode.overview, Episode.guestStars, Episode.writer, Episode.watched "
                           "FROM Series, Episode "
                           "WHERE Series.status = '%1' AND Episode.firstAired BETWEEN '%2' AND '%3' AND Series.id = Episode.seriesID AND Episode.seasonNumber != 0 "
                           "ORDER BY Episode.firstAired;").arg(status).arg(firstAiredStart).arg(firstAiredEnd));
        
        if (query.isSelect()) {
            
            while (query.next()) {
                
                QVariantMap temp;
                
                auto seriesName = query.value(0).toString();
                temp["seriesName"] = seriesName;
                
                auto network = query.value(1).toString();
                temp["network"] = network;
                
                auto airsTime = query.value(2).toString();
                QTime time;

                if (airsTime.contains("PM")) {
                    airsTime.resize(airsTime.indexOf("PM") - 1);
                    time = QTime::fromString(airsTime, "h:m").addSecs(kTwelveHoursInSecs);
                } else {
                    time = QTime::fromString(airsTime, "h:m");
                }
                
                temp["airsTime"] = time.toString("h:mm");
                
                auto airsDayOfWeek = query.value(3).toString();
                temp["airsDayOfWeek"] = airsDayOfWeek;
                
                auto status = query.value(4).toString();
                temp["status"] = status;
                
                auto id = query.value(5).toString();
                temp["id"] = id;
                
                auto episodeId = query.value(6).toString();
                temp["nextEpisodeId"] = episodeId;
                
                auto episodeName = query.value(7).toString();
                temp["nextEpisodeName"] = episodeName;
                
                auto episodeNumber = query.value(8).toString();
                temp["nextEpisodeNumber"] = episodeNumber;
                
                auto seasonNumber = query.value(9).toString();
                temp["nextEpisodeSeasonNumber"] = seasonNumber;
                
                auto firstAired = query.value(10).toString();
                temp["nextEpisodeFirstAired"] = firstAired;
                
                // Sometimes the series info airsDayOfWeek is wrong so lets take it from the episode directly then
                auto airsDayOfWeekFromEpisode = locale.toString(QDate::fromString(firstAired, Qt::ISODate), "dddd");
                temp["airsDayOfWeek"] = airsDayOfWeek == airsDayOfWeekFromEpisode ? airsDayOfWeek : airsDayOfWeekFromEpisode;
                
                auto banner = query.value(11).toString();
                temp["nextEpisodeBanner"] = banner;
                
                auto overview = query.value(12).toString();
                overview.replace("''","'");
                temp["nextEpisodeOverview"] = overview;
                
                auto guestStars = query.value(13).toString();
                temp["nextEpisodeGuestStars"] = guestStars;
                
                auto writer = query.value(14).toString();
                temp["nextEpisodeWriter"] = writer;
                
                auto watched = query.value(15).toString();
                temp["nextEpisodeWatched"] = watched;
                
                series.append(temp);
            }
        }
    }

    return series;
}

QList<QVariantMap> DatabaseManager::getEpisodes(int seriesID, int seasonNumber) {

    QList<QVariantMap> episodes;

    QSqlQuery query(m_db);
    query.exec(QString("SELECT episodeName, episodeNumber, overview, seasonNumber, absoluteNumber, filename, watched, id, guestStars, writer, firstAired "
                       "FROM Episode "
                       "WHERE seriesID = %1 AND seasonNumber = %2 "
                       "ORDER BY episodeNumber").arg(seriesID).arg(seasonNumber));
    
    if (query.isSelect()) {
        
        while (query.next()) {

            QVariantMap episode;

            QString episodeName = query.value(0).toString();
            episode["episodeName"] = episodeName;

            QString episodeNumber = query.value(1).toString();
            episode["episodeNumber"] = episodeNumber;

            QString overview = query.value(2).toString();
            overview.replace("''","'");
            episode["overview"] = overview;

            QString seasonNumber = query.value(3).toString();
            episode["seasonNumber"] = seasonNumber;

            QString absoluteNumber = query.value(4).toString();
            episode["absoluteNumber"] = absoluteNumber;

            QString filename = query.value(5).toString();
            episode["filename"] = filename;

            QString watched = query.value(6).toString();
            episode["watched"] = watched;

            QString id = query.value(7).toString();
            episode["id"] = id;

            QString guestStars = query.value(8).toString();
            episode["guestStars"] = guestStars;

            QString writer = query.value(9).toString();
            episode["writer"] = writer;

            QString firstAired = query.value(10).toString();
            episode["firstAired"] = firstAired;

            episodes.append(episode);
        }
    }
    return episodes;
}

void DatabaseManager::toggleWatched(QString episodeID)
{
    QSqlQuery query(m_db);
    query.exec(QString("UPDATE Episode "
                       "SET watched = CASE WHEN watched = 0 THEN 1 ELSE 0 END "
                       "WHERE id = %1").arg(episodeID.toInt()));
}

bool DatabaseManager::deleteSeries(int seriesID)
{
    auto ret1 = false;
    auto ret2 = false;
    auto ret3 = false;
    
    this->startTransaction();
    
    QSqlQuery query(m_db);
    ret1 = query.exec(QString("DELETE FROM Series WHERE id = %1").arg(seriesID));
    if (ret1) {
        ret2 = query.exec(QString("DELETE FROM Episode WHERE seriesID = %1").arg(seriesID));
        if (ret1 && ret2) {
            ret3 = query.exec(QString("DELETE FROM Banner WHERE seriesID = %1").arg(seriesID));
        }
    }
    
    this->commit();
    
    return ret1 && ret2 && ret3;
}

bool DatabaseManager::deleteAllSeries()
{
    auto ret1 = false;
    auto ret2 = false;
    auto ret3 = false;
    
    this->startTransaction();
    
    QSqlQuery query(m_db);
    ret1 = query.exec(QString("DELETE FROM Series"));
    if (ret1) {
        ret2 = query.exec(QString("DELETE FROM Episode"));
        if (ret1 && ret2) {
            ret3 = query.exec(QString("DELETE FROM Banner"));
        }
    }
    
    this->commit();
    
    return ret1 && ret2 && ret3;
}

bool DatabaseManager::isAlreadyAdded(int seriesId, QString name)
{
    auto ret1 = false;
    auto ret2 = false;
    
    QSqlQuery query(m_db);
    query.prepare("SELECT seriesName FROM Series WHERE id = :seriesId");
    query.bindValue(":seriesId", seriesId);
    ret1 = query.exec();
    if (ret1) {
        if (query.isSelect()) {
            while (query.next()) {
                if (query.value(0).toString() == name) {
                    ret2 = true;
                }
            }
        }
    }
    return ret2;
}

int DatabaseManager::watchedCount(int seriesID)
{
    auto watchedCount = 0;
    QSqlQuery query(m_db);
    query.exec(QString("SELECT COUNT(episodeName) "
                       "FROM Episode "
                       "WHERE seriesID = %1 AND watched = 1 AND seasonNumber != 0;").arg(seriesID));
    if (query.isSelect()) {
        while (query.next()) {
            watchedCount = query.value(0).toInt();
        }
    }
    return watchedCount;
}

int DatabaseManager::watchedCountBySeason(int seriesID, int seasonNumber)
{
    auto watchedCount = 0;
    QSqlQuery query(m_db);
    query.exec(QString("SELECT COUNT(episodeName) "
                       "FROM Episode "
                       "WHERE seriesID = %1 AND watched = 1 AND seasonNumber = %2;").arg(seriesID).arg(seasonNumber));
    if (query.isSelect()) {
        while (query.next()) {
            watchedCount = query.value(0).toInt();
        }
    }
    return watchedCount;
}

int DatabaseManager::totalCount(int seriesID)
{
    auto totalCount = 0;
    QSqlQuery query(m_db);
    query.exec(QString("SELECT COUNT(episodeName) "
                       "FROM Episode "
                       "WHERE seriesID = %1 AND seasonNumber != 0;").arg(seriesID));
    if (query.isSelect()) {
        while (query.next()) {
            totalCount = query.value(0).toInt();
        }
    }
    return totalCount;
    
}

int DatabaseManager::totalCountBySeason(int seriesID, int seasonNumber)
{
    auto totalCount = 0;
    QSqlQuery query(m_db);
    query.exec(QString("SELECT COUNT(episodeName) "
                       "FROM Episode "
                       "WHERE seriesID = %1 AND seasonNumber = %2;").arg(seriesID).arg(seasonNumber));
    if (query.isSelect()) {
        while (query.next()) {
            totalCount = query.value(0).toInt();
        }
    }
    return totalCount;
}

int DatabaseManager::seasonCount(int seriesID)
{
    auto seasonCount = 0;
    QSqlQuery query(m_db);
    query.exec(QString("SELECT MAX(seasonNumber) "
                       "FROM Episode "
                       "WHERE seriesID = %1;").arg(seriesID));
    if (query.isSelect()) {
        while (query.next()) {
            seasonCount = query.value(0).toInt();
        }
    }
    return seasonCount;
}

void DatabaseManager::markSeasonWatched(int seriesID, int season)
{
    QSqlQuery query(m_db);
    query.exec(QString("UPDATE Episode "
                       "SET watched = 1 "
                       "WHERE seriesID = %1 AND seasonNumber = %2;").arg(seriesID).arg(season));
}

QVariantMap DatabaseManager::getNextEpisodeDetails(int seriesID)
{
    auto today = QDateTime::currentDateTime().date().toString(Qt::ISODate);
    
    QList<QVariantMap> details;
    
    this->startTransaction();

    QSqlQuery query(m_db);
    query.exec(QString("SELECT episodeName, episodeNumber, seasonNumber, firstAired "
                       "FROM Episode WHERE seriesID = %1 AND seasonNumber != 0 AND firstAired >= '%2' "
                       "ORDER BY episodeNumber "
                       "LIMIT 1;").arg(seriesID).arg(today));
    this->commit();
    
    if (query.isSelect()) {
        
        while (query.next()) {
            
            QVariantMap temp;
            
            auto episodeName = query.value(0).toString();
            temp["nextEpisodeName"] = episodeName;
            
            auto episodeNumber = query.value(1).toString();
            temp["nextEpisodeNumber"] = episodeNumber;
            
            auto seasonNumber = query.value(2).toString();
            temp["nextEpisodeSeasonNumber"] = seasonNumber;
            
            auto firstAired = query.value(3).toString();
            temp["nextEpisodeFirstAired"] = firstAired;
            
            details.append(temp);
        }
    }
    
    QVariantMap nextEpisodeDetails;
    
    if (!details.isEmpty()) {
        
        nextEpisodeDetails = details.first();
        
        auto firstAired = nextEpisodeDetails["nextEpisodeFirstAired"].toString();
        auto daysToNextEpisode = QDate::fromString(today, "yyyy-MM-dd").daysTo(QDate::fromString(firstAired, "yyyy-MM-dd"));
        
        switch (daysToNextEpisode) {
        case 0:
            nextEpisodeDetails["daysToNext"] = "today";
            break;
        case 1:
            nextEpisodeDetails["daysToNext"] = "tomorrow";
            break;
        default:
            nextEpisodeDetails["daysToNext"] = QString::number(daysToNextEpisode);
            break;
        }
    }
    
    return nextEpisodeDetails;
}

QString DatabaseManager::getStatus(int seriesID)
{
    QSqlQuery query(m_db);
    QString status;
    query.exec(QString("SELECT status FROM Series WHERE id = %1;").arg(seriesID));
    if (query.isSelect()) {
        while (query.next()) {
            status = query.value(0).toString();
        }
    }
    return status;
}

QString DatabaseManager::getSeasonBanner(int seriesID, int season)
{
    QSqlQuery query(m_db);
    QString banner = "";
    auto bannerType = "season";
    query.exec(QString("SELECT bannerPath "
                       "FROM Banner "
                       "WHERE seriesID = %1 AND bannerType = '%2' AND season = %3;").arg(seriesID).arg(bannerType).arg(season));
    if (query.isSelect()) {
        while (query.next()) {
            banner = query.value(0).toString();
        }
    }
    return banner;
}

int DatabaseManager::getWatchedEpisodesDuration()
{
    auto duration = 0;

    QSqlQuery query(m_db);
    query.exec("SELECT SUM(runtime) "
               "FROM series "
               "LEFT JOIN episode ON episode.seriesID = series.id AND episode.watched = 1;");

    if (query.isSelect()) {
        while (query.next()) {
            duration = query.value(0).toInt();
        }
    }
    return duration;
}

double DatabaseManager::getAverageWatchedEpisodesDuration()
{
    auto duration = 0;

    QSqlQuery query(m_db);
    query.exec("SELECT AVG(runtime) "
               "FROM series "
               "LEFT JOIN episode ON episode.seriesID = series.id AND episode.watched = 1;");

    if (query.isSelect()) {
        while (query.next()) {
            duration = query.value(0).toDouble();
        }
    }
    return duration;
}

int DatabaseManager::getWatchedEpisodesCount()
{
    auto count = 0;

    QSqlQuery query(m_db);
    query.exec("SELECT COUNT(*) "
               "FROM episode "
               "WHERE episode.watched = 1;");

    if (query.isSelect()) {
        while (query.next()) {
            count = query.value(0).toInt();
        }
    }
    return count;
}

int DatabaseManager::getAllEpisodesCount()
{
    auto count = 0;

    QSqlQuery query(m_db);
    query.exec("SELECT COUNT(*) "
               "FROM episode "
               "WHERE episode.seasonNumber != 0;");

    if (query.isSelect()) {
        while (query.next()) {
            count = query.value(0).toInt();
        }
    }
    return count;
}

int DatabaseManager::getAllSeriesCount()
{
    auto count = 0;

    QSqlQuery query(m_db);
    query.exec("SELECT COUNT(*) FROM series;");

    if (query.isSelect()) {
        while (query.next()) {
            count = query.value(0).toInt();
        }
    }
    return count;
}

int DatabaseManager::getEndedSeriesCount()
{
    auto count = 0;

    QSqlQuery query(m_db);
    query.exec("SELECT COUNT(*) "
               "FROM series "
               "WHERE series.status = 'Ended';");

    if (query.isSelect()) {
        while (query.next()) {
            count = query.value(0).toInt();
        }
    }
    return count;
}

int DatabaseManager::getWatchedSeriesCount()
{
    auto inProgressSeriesCount = 0;
    auto allSeriesCount = 0;
    QSqlQuery query(m_db);

    query.exec("SELECT COUNT (DISTINCT seriesID) "
               "FROM "
               "(SELECT episode.id, episode.seriesID "
               "FROM episode, series "
               "WHERE episode.seriesID = series.id AND episode.watched = 0 and episode.seasonNumber != 0);");

    if (query.isSelect()) {
        while (query.next()) {
            inProgressSeriesCount = query.value(0).toInt();
        }
    }

    query.exec("SELECT COUNT(id) FROM series;");

    if (query.isSelect()) {
        while (query.next()) {
            allSeriesCount = query.value(0).toInt();
        }
    }
    return allSeriesCount - inProgressSeriesCount;
}

int DatabaseManager::getWatchedSeasonsCount()
{
    auto count = 0;

    QSqlQuery query(m_db);
    query.exec("SELECT * "
               "FROM (SELECT MAX(episodeNumber) AS episodes, SUM(watched) AS watched, seasonID FROM episode GROUP BY seasonID) "
               "GROUP BY seasonID "
               "HAVING episodes = watched;");

    if (query.isSelect()) {
        while (query.next()) {
            count += 1;
        }
    }
    return count;
}

int DatabaseManager::getAllSeasonsCount()
{
    auto count = 0;

    QSqlQuery query(m_db);
    query.exec("SELECT SUM(seasonCount) "
               "FROM (SELECT MAX(seasonNumber) AS seasonCount FROM episode GROUP BY seriesID);");

    if (query.isSelect()) {
        while (query.next()) {
            count = query.value(0).toInt();
        }
    }
    return count;
}

QMultiMap<int, QMap<QString, QStringList> > DatabaseManager::getMostWatchedDirectors()
{
    QList<QMap<QString, QString> > directors;

    QSqlQuery query(m_db);
    query.exec("SELECT director, seriesName "
               "FROM series "
               "LEFT JOIN episode "
               "ON episode.seriesID = series.id AND episode.watched = 1;");

    if (query.isSelect()) {
        while (query.next()) {
            auto directorString = query.value(0).toString();
            auto seriesName = query.value(1).toString();

            if (directorString.contains("|")) {
                auto parsed = directorString.split("|");
                parsed.removeAll("");
                foreach (auto director, parsed) {
                    QMap<QString, QString> temp;
                    temp.insert(director, seriesName);
                    directors.append(temp);
                }
            } else {
                QMap<QString, QString> temp;
                temp.insert(directorString, seriesName);
                directors.append(temp);
            }
        }
    }

    QMap<QString, QStringList> occurences;
    foreach (auto item, directors) {
        occurences[item.firstKey()].append(item.first());
    }

    QMultiMap<int, QMap<QString, QStringList> > result;
    auto itr = occurences.begin();
    while (itr != occurences.end()) {
        auto size = itr.value().size();
        QMap<QString, QStringList> temp;
        temp.insert(itr.key(), itr.value());
        result.insert(size, temp);
        ++itr;
    }

    qDebug() << result;
    return result;
}

QMultiMap<int, QMap<QString, QStringList> > DatabaseManager::getMostWatchedActors()
{
    QList<QMultiMap<QString, QString> > actors;

    QSqlQuery query(m_db);
    query.exec("SELECT actors, seriesName FROM series;");

    if (query.isSelect()) {
        while (query.next()) {

            auto line = query.value(0).toString();
            auto seriesName = query.value(1).toString();

            auto parsed = line.split("|");
            parsed.removeAll("");
            foreach (auto actor, parsed) {
                QMultiMap<QString, QString> temp;
                temp.insert(actor, seriesName);
                actors.append(temp);
            }
        }
    }

    QMap<QString, QStringList> occurences;
    foreach (auto item, actors) {
        occurences[item.firstKey()].append(item.first());
    }

    QMultiMap<int, QMap<QString, QStringList> > result;
    auto itr = occurences.begin();
    while (itr != occurences.end()) {
        auto size = itr.value().size();
        QMap<QString, QStringList> temp;
        temp.insert(itr.key(), itr.value());
        result.insert(size, temp);
        ++itr;
    }

    qDebug() << result;
    return result;
}
