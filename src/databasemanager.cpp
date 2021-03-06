#include "databasemanager.h"

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{
    setUpDB();
}

DatabaseManager::~DatabaseManager()
{
    this->close();
}

void DatabaseManager::setUpDB()
{
    if (openDB())
    {
        if (m_db.tables().size() == 0)
        {
            createDB();
            
        }
        else
        {
            
            QSqlQuery query(m_db);
            query.exec("SELECT name, version FROM information");
            if (query.isSelect())
            {
                while (query.next())
                {
                    auto name = query.value(0).toString();
                    auto version = query.value(1).toDouble();
                    qDebug() << "App name: " << name;
                    qDebug() << "Database version: " << version;
                    
                    if (version == 1.0)
                    {
                        if (createBannerTable())
                        {
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
    auto dbname = "sailSeries.db.sqlite";
    auto dbpath = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + dbname;
    QDir dir(QStandardPaths::writableLocation(QStandardPaths::DataLocation));
    
    if (!dir.exists()) {
        dir.mkpath(".");
    }
    
    m_db = QSqlDatabase::addDatabase("QSQLITE", "databasemanager");
    m_db.setDatabaseName(dbpath);
    
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
    m_db.close();
    
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

bool DatabaseManager::rollback()
{
    return m_db.rollback();
}

bool DatabaseManager::initializeInfoTable()
{
    bool ret = false;

    if (createInfoTable())
    {
        QSqlQuery query(m_db);
        query.prepare("INSERT INTO information VALUES(:version, :name)");
        query.bindValue(":version", 1.0);
        query.bindValue(":name", "SailSeries");
        ret = query.exec();
    }

    return ret;
}

bool DatabaseManager::createInfoTable()
{
    bool result = false;
    
    if (m_db.isOpen())
    {
        QSqlQuery query(m_db);
        result = query.exec("CREATE TABLE information"
                         "(version real primary key, "
                         "name varchar(50))");
    }

    return result;
}

bool DatabaseManager::updateInfoTable(double version)
{
    bool result = false;

    if (m_db.isOpen())
    {
        QSqlQuery query(m_db);
        query.prepare("UPDATE information SET version = :version");
        query.bindValue(":version", version);
        result = query.exec();
    }

    return result;
}

bool DatabaseManager::createDB()
{    
    if (m_db.isOpen())
    {
        
        if (initializeInfoTable())
        {
            qDebug() << "information table created";
        }
        
        if (createSeriesTable() && createEpisodeTable() && createBannerTable())
        {
            qDebug() << "Tables created";   
            updateInfoTable(2.0);
        }
    }

    return true;
}

bool DatabaseManager::createSeriesTable()
{
    bool result = false;
    if (m_db.isOpen())
    {
        QSqlQuery query(m_db);
        result = query.exec("CREATE TABLE Series"
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
                         "watched INTEGER DEFAULT 0)");
    }

    return result;
}

bool DatabaseManager::createEpisodeTable()
{
    bool result = false;
    if (m_db.isOpen())
    {
        QSqlQuery query(m_db);
        result = query.exec("CREATE TABLE Episode"
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
                         "watched INTEGER DEFAULT 0)");
    }

    return result;
}

bool DatabaseManager::createBannerTable()
{
    bool result = false;
    if (m_db.isOpen())
    {
        QSqlQuery query(m_db);
        result = query.exec("CREATE TABLE Banner"
                         "(id INTEGER PRIMARY KEY, "
                         "seriesID INTEGER, "
                         "bannerPath VARCHAR(50), "
                         "bannerType VARCHAR(50), "
                         "bannerType2 VARCHAR(50), "
                         "language VARCHAR(2), "
                         "season INTEGER)");
    }

    return result;
}

void DatabaseManager::deleteDuplicateEpisodes()
{
    if (m_db.isOpen())
    {
        QSqlQuery query(m_db);
        query.exec("DELETE FROM episode "
                   "WHERE (firstAired = '' OR firstAired IS NULL) AND (episodeName IS NULL OR episodeName = '')");
    }
}

void DatabaseManager::storeSeries(const QVariantMap &series)
{   
    auto id = series["id"].toInt();
    auto actors = series["actors"].toString();
    auto airsDayOfWeek = series["airsDayOfWeek"].toString();
    auto airsTime = series["airsTime"].toString();
    auto contentRating = series["contentRating"].toString();
    auto firstAired = series["firstAired"].toString();
    auto genre = series["genre"].toStringList().join(",");
    auto imdbId = series["imdbId"].toString();
    auto language = series["language"].toString();
    auto network = series["network"].toString();
    auto overview = series["overview"].toString();
    auto rating = series["siteRating"].toDouble();
    auto ratingCount = series["siteRatingCount"].toInt();
    auto runtime = series["runtime"].toInt();
    auto seriesName = series["seriesName"].toString();
    auto status = series["status"].toString();
    auto added = series["added"].toString();
    auto addedBy = series["addedBy"].toInt();
    auto banner = series["banner"].toString();
    auto fanart = series["fanart"].toString();
    auto lastUpdated = series["lastUpdated"].toString();
    auto poster = series["poster"].toString();
    auto zap2itId = series["zap2itId"].toString();
    auto watched = 0;
    
    overview.replace("'", "''");
    actors.replace("'", "''");
    seriesName.replace("'", "''");

    startTransaction();
        
    if (m_db.isOpen())
    {
        QSqlQuery query(m_db);
        query.prepare("INSERT OR REPLACE INTO Series(id, actors, airsDayOfWeek, airsTime, contentRating, firstAired, genre, imdbID, language, network, overview, rating, ratingCount, runtime, seriesName, status, added, addedBy, banner, fanart, lastupdated, poster, zap2itID, watched) "
                      "VALUES(:id, :actors, :airsDayOfWeek, :airsTime, :contentRating, :firstAired, :genre, :imdbId, :language, :network, :overview, :rating, :ratingCount, :runtime, :seriesName, :status, :added, :addedBy, :banner, :fanart, :lastUpdated, :poster, :zap2itId, :watched)");
        query.bindValue(":id", id);
        query.bindValue(":actors", actors);
        query.bindValue(":airsDayOfWeek", airsDayOfWeek);
        query.bindValue(":airsTime", airsTime);
        query.bindValue(":contentRating", contentRating);
        query.bindValue(":firstAired", firstAired);
        query.bindValue(":genre", genre);
        query.bindValue(":imdbId", imdbId);
        query.bindValue(":language", language);
        query.bindValue(":network", network);
        query.bindValue(":overview", overview);
        query.bindValue(":rating", rating);
        query.bindValue(":ratingCount", ratingCount);
        query.bindValue(":runtime", runtime);
        query.bindValue(":seriesName", seriesName);
        query.bindValue(":status", status);
        query.bindValue(":added", added);
        query.bindValue(":addedBy", addedBy);
        query.bindValue(":banner", banner);
        query.bindValue(":fanart", fanart);
        query.bindValue(":lastUpdated", lastUpdated);
        query.bindValue(":poster", poster);
        query.bindValue(":zap2itId", zap2itId);
        query.bindValue(":watched", watched);
        query.exec();
        
        qDebug() << query.lastError();
    }
    
    commit();
    emit seriesStored(id);
}

void DatabaseManager::storeEpisodes(const int &seriesId, const QList<QVariantMap> &episodes)
{
    startTransaction();

    for (auto episode : episodes)
    {
        auto absoluteNumber = episode["absoluteNumber"].toInt();
        auto filename = episode["filename"].toString();
        auto episodeNumber = episode["airedEpisodeNumber"].toInt();
        auto seasonNumber = episode["airedSeason"].toInt();
        auto seasonId = episode["airedSeasonID"].toInt();
        auto episodeName = episode["episodeName"].toString();
        auto firstAired = episode["firstAired"].toString();
        auto id = episode["id"].toInt();
        auto language = episode["language"].toMap()["overview"].toString();
        auto lastUpdated = episode["lastUpdated"].toString();
        auto overview = episode["overview"].toString();
        auto guestStars = episode["guestStars"].toStringList().join(", ");
        auto writers = episode["writers"].toStringList().join(", ");
        auto watched = 0;

        // important!
        overview.replace("'", "''");
        episodeName.replace("'", "''");
        
        if (m_db.isOpen())
        {
            // lets get and check the watched flag first, might be slow but this must be done
            // in order to keep the flag state
            QSqlQuery query(m_db);
            query.prepare("SELECT watched FROM Episode WHERE id = :id");
            query.bindValue(":id", id);
            query.exec();
            
            if (query.isSelect())
            {
                while (query.next())
                {
                    watched = query.value(0).toInt();
                }
            }
            
            query.prepare("INSERT OR REPLACE INTO Episode (id, episodeName, episodeNumber, firstAired, guestStars, language, overview, seasonNumber, writer, absoluteNumber, filename, lastupdated, seasonID, seriesID, watched) "
                          "VALUES (:id, :episodeName, :episodeNumber, :firstAired, :guestStars, :language, :overview, :seasonNumber, :writer, :absoluteNumber, :filename, :lastUpdated, :seasonId, :seriesId, :watched)");
            query.bindValue(":id", id);
            query.bindValue(":episodeName", episodeName);
            query.bindValue(":episodeNumber", episodeNumber);
            query.bindValue(":firstAired", firstAired);
            query.bindValue(":guestStars", guestStars);
            query.bindValue(":language", language);
            query.bindValue(":overview", overview);
            query.bindValue(":seasonNumber", seasonNumber);
            query.bindValue(":writer", writers);
            query.bindValue(":absoluteNumber", absoluteNumber);
            query.bindValue(":filename", filename);
            query.bindValue(":lastUpdated", lastUpdated);
            query.bindValue(":seasonId", seasonId);
            query.bindValue(":seriesId", seriesId);
            query.bindValue(":watched", watched);
            query.exec();

            qDebug() << query.lastError();
        }
    }

    commit();
}

void DatabaseManager::storeSeasonImages(const int &seriesId, const QList<QVariantMap> &banners)
{
    startTransaction();

    for (auto banner : banners)
    {
        const auto id = banner["id"].toInt();
        const auto bannerPath = banner["bannerPath"].toString();
        const auto bannerType = banner["bannerType"].toString();
        const auto bannerType2 = banner["bannerType2"].toString();
        const auto language = banner["language"].toString();
        const auto season = banner["season"].toInt();
        
        if (m_db.isOpen())
        {
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
            query.exec();

            qDebug() << query.lastError();
        }
    }
    
    commit();
}

void DatabaseManager::getSeriesIds(const bool &includeEndedSeries)
{
    QList<int> seriesIds;
    QString queryString = [=]()
    {
        if (includeEndedSeries)
        {
            return "SELECT id "
                   "FROM Series "
                   "ORDER BY seriesName";
        }

        return "SELECT id "
               "FROM Series "
               "WHERE status != 'Ended' "
               "ORDER BY seriesName";
    }();

    if (m_db.isOpen())
    {
        startTransaction();

        QSqlQuery query(m_db);
        query.exec(queryString);

        qDebug() << query.lastError();

        commit();

        if (query.isSelect())
        {
            while (query.next())
            {
                seriesIds.append(query.value(0).toInt());
            }
        }
    }

    emit getSeriesIdsReady(seriesIds);
}

void DatabaseManager::getSeries()
{
    QList<QVariantMap> allSeries;
    
    if (m_db.isOpen())
    {
        QSqlQuery query(m_db);
        query.exec("SELECT banner, poster, seriesName, status, id, overview, imdbID, rating, genre "
                   "FROM Series "
                   "ORDER BY seriesName");

        qDebug() << query.lastError();
        
        if (query.isSelect())
        {
            while (query.next())
            {
                QVariantMap series;
                
                const auto banner = query.value(0).toString();
                series["banner"] = banner;
                
                const auto poster = query.value(1).toString();
                series["poster"] = poster;
                
                auto seriesName = query.value(2).toString();
                seriesName.replace("''", "'");
                series["seriesName"] = seriesName;
                
                const auto status = query.value(3).toString();
                series["status"] = status;
                
                const auto id = query.value(4).toInt();
                series["id"] = QString::number(id);
                
                auto overview = query.value(5).toString();
                overview.replace("''", "'");
                series["overview"] = overview;
                
                const auto imdbId = query.value(6).toString();
                series["imdbId"] = imdbId;
                
                const auto rating = query.value(7).toString();
                series["rating"] = rating;
                
                const auto genre = query.value(8).toString();
                series["genre"] = genre;
                
                const auto watchedCount = QString::number(this->watchedCount(id));
                series["watchedCount"] = watchedCount;
                
                const auto totalCount = QString::number(this->totalCount(id));
                series["totalCount"] = totalCount;

                // enrich with next episode's details
                series.unite(getNextEpisodeDetails(id));
                
                allSeries.append(series);
            }
        }
    }

    emit populateBannerList(allSeries);
}

void DatabaseManager::getStartPageSeries()
{
    const auto date = QDateTime::currentDateTime().date();
    const auto locale  = QLocale(QLocale::English);
    const auto firstAiredStart = date.addDays(1 - date.dayOfWeek()).toString(Qt::ISODate); // Monday == 1
    const auto firstAiredEnd = date.addDays(7 - date.dayOfWeek()).toString(Qt::ISODate); // Sunday == 7
    const auto status = "Continuing";
    const auto kTwelveHoursInSecs = 12 * 60 * 60;
    
    QList<QVariantMap> allSeries;
    
    if (m_db.isOpen())
    {
        QSqlQuery query(m_db);
        query.prepare("SELECT Series.seriesName, Series.network, Series.airsTime, Series.airsDayOfWeek, Series.status, Series.id, Episode.id, Episode.episodeName, Episode.episodeNumber, Episode.seasonNumber, Episode.firstAired, Episode.filename, Episode.overview, Episode.guestStars, Episode.writer, Episode.watched, Episode.filename "
                      "FROM Series, Episode "
                      "WHERE Series.status = :status AND Episode.firstAired BETWEEN :firstAiredStart AND :firstAiredEnd AND Series.id = Episode.seriesID "
                      "ORDER BY Episode.firstAired");
        query.bindValue(":status", status);
        query.bindValue(":firstAiredStart", firstAiredStart);
        query.bindValue(":firstAiredEnd", firstAiredEnd);
        query.exec();

        qDebug() << query.lastError();
        
        if (query.isSelect())
        {
            while (query.next())
            {
                QVariantMap series;
                
                const auto seriesName = query.value(0).toString();
                series["seriesName"] = seriesName;
                
                const auto network = query.value(1).toString();
                series["network"] = network;
                
                auto airsTime = query.value(2).toString();
                const auto time = [&airsTime]()
                {
                    if (airsTime.contains("PM"))
                    {
                        airsTime.resize(airsTime.indexOf("PM") - 1);
                        return QTime::fromString(airsTime, "h:m").addSecs(kTwelveHoursInSecs);
                    }

                    return QTime::fromString(airsTime, "h:m");
                }();
                series["airsTime"] = time.toString("h:mm");
                
                const auto airsDayOfWeek = query.value(3).toString();
                series["airsDayOfWeek"] = airsDayOfWeek;
                
                const auto status = query.value(4).toString();
                series["status"] = status;
                
                const auto id = query.value(5).toString();
                series["id"] = id;
                
                const auto episodeId = query.value(6).toString();
                series["nextEpisodeId"] = episodeId;
                
                auto episodeName = query.value(7).toString();
                episodeName.replace("''", "'");
                series["nextEpisodeName"] = episodeName;
                
                const auto episodeNumber = query.value(8).toString();
                series["nextEpisodeNumber"] = episodeNumber;
                
                const auto seasonNumber = query.value(9).toString();
                series["nextEpisodeSeasonNumber"] = seasonNumber;
                
                const auto firstAired = query.value(10).toString();
                series["nextEpisodeFirstAired"] = firstAired;
                
                // Sometimes the series' airsDayOfWeek is wrong so lets take it from the episode directly in that case
                series["airsDayOfWeek"] = [locale, firstAired, airsDayOfWeek]()
                {
                    auto airsDayOfWeekFromEpisode = locale.toString(QDate::fromString(firstAired, Qt::ISODate), "dddd");
                    return airsDayOfWeek == airsDayOfWeekFromEpisode ? airsDayOfWeek : airsDayOfWeekFromEpisode;
                }();
                
                const auto banner = query.value(11).toString();
                series["nextEpisodeBanner"] = banner;
                
                auto overview = query.value(12).toString();
                overview.replace("''", "'");
                series["nextEpisodeOverview"] = overview;
                
                const auto guestStars = query.value(13).toString();
                series["nextEpisodeGuestStars"] = guestStars;
                
                const auto writer = query.value(14).toString();
                series["nextEpisodeWriter"] = writer;
                
                const auto watched = query.value(15).toString();
                series["nextEpisodeWatched"] = watched;

                const auto filename = query.value(16).toString();
                series["nextEpisodeFilename"] = filename;
                
                allSeries.append(series);
            }
        }
    }

    emit populateTodayModel(allSeries);
}

void DatabaseManager::getSeasons(const int &seriesId)
{
    QList<QVariantMap> seasons;

    const int seasonsCount = seasonCount(seriesId);
    for (int i = 0; i <= seasonsCount; ++i) {
        QVariantMap season;
        season["banner"] = getSeasonBanner(seriesId, i);
        season["watchedCount"] = watchedCountBySeason(seriesId, i);
        season["totalCount"] = totalCountBySeason(seriesId, i);
        seasons.append(season);
    }

    emit populateSeasonList(seasons);
}

void DatabaseManager::getEpisodes(const int &seriesId, const int &seasonNumber)
{
    QList<QVariantMap> episodes;
    
    QSqlQuery query(m_db);
    query.prepare("SELECT episodeName, episodeNumber, overview, seasonNumber, absoluteNumber, filename, watched, id, guestStars, writer, firstAired "
                  "FROM Episode "
                  "WHERE seriesID = :seriesId AND seasonNumber = :seasonNumber "
                  "ORDER BY episodeNumber");
    query.bindValue(":seriesId", seriesId);
    query.bindValue(":seasonNumber", seasonNumber);
    query.exec();
    
    if (query.isSelect())
    {
        while (query.next())
        {
            QVariantMap episode;

            QString episodeName = query.value(0).toString();
            episodeName.replace("''", "'");
            episode["episodeName"] = episodeName;

            QString episodeNumber = query.value(1).toString();
            episode["episodeNumber"] = episodeNumber;

            QString overview = query.value(2).toString();
            overview.replace("''", "'");
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

    emit populateEpisodeList(episodes);
}

void DatabaseManager::setWatched(const int &episodeId, const int &seriesId, const bool &watched)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Episode "
                  "SET watched = :watched "
                  "WHERE id = :episodeId");
    query.bindValue(":watched", watched ? 1 : 0);
    query.bindValue(":episodeId", episodeId);
    auto result = query.exec();

    qDebug() << query.lastError();

    if (result == true)
    {
        getSeasons(seriesId);
        getStartPageSeries();
        emit setWatchedReady(episodeId, watched);
    }
}

void DatabaseManager::storePosterImageFor(const int &seriesId, const QString &posterImage) const
{
    QSqlQuery query(m_db);
    query.prepare(QString("UPDATE Series "
                          "SET poster = :poster "
                          "WHERE id = :seriesId"));
    query.bindValue(":poster", posterImage);
    query.bindValue(":seriesId", seriesId);
    query.exec();

    qDebug() << query.lastError();
}

void DatabaseManager::storeBannerImageFor(const int &seriesId, const QString &bannerImage) const
{
    QSqlQuery query(m_db);
    query.prepare(QString("UPDATE Series "
                          "SET banner = :banner "
                          "WHERE id = :seriesId"));
    query.bindValue(":banner", bannerImage);
    query.bindValue(":seriesId", seriesId);
    query.exec();

    qDebug() << query.lastError();
}

void DatabaseManager::storeFanartImageFor(const int &seriesId, const QString &fanartImage) const
{
    QSqlQuery query(m_db);
    query.prepare(QString("UPDATE Series "
                          "SET fanart = :fanart "
                          "WHERE id = :seriesId"));
    query.bindValue(":fanart", fanartImage);
    query.bindValue(":seriesId", seriesId);
    query.exec();

    qDebug() << query.lastError();
}

void DatabaseManager::storeActors(const int &seriesId, const QList<QVariantMap> &actors) const
{
    auto actorNames = QString("");

    for (auto actor : actors)
    {
        actorNames += QString("|%1").arg(actor["name"].toString());
    }

    QSqlQuery query(m_db);
    query.prepare("UPDATE Series "
                  "SET actors = :actorNames "
                  "WHERE id = :seriesId");
    query.bindValue(":actors", actorNames);
    query.bindValue(":seriesId", seriesId);
    query.exec();

    qDebug() << query.lastError();
}

void DatabaseManager::deleteSeries(const int &seriesId)
{
    auto seriesDeleted = false;
    auto episodesDeleted = false;
    auto bannersDeleted = false;
    
    this->startTransaction();
    
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Series WHERE id = :seriesId");
    query.bindValue(":seriesId", seriesId);
    seriesDeleted = query.exec();

    if (seriesDeleted)
    {
        query.prepare("DELETE FROM Episode WHERE seriesID = :seriesId");
        query.bindValue(":seriesId", seriesId);
        episodesDeleted = query.exec();

        if (seriesDeleted && episodesDeleted)
        {
            query.prepare("DELETE FROM Banner WHERE seriesID = :seriesId");
            query.bindValue(":series", seriesId);
            bannersDeleted = query.exec();
        }
    }

    if (seriesDeleted && episodesDeleted && bannersDeleted)
    {
        this->commit();
        emit this->seriesDeleted();
    }
    else
    {
        this->rollback();
    }
}

bool DatabaseManager::deleteAllSeries()
{
    auto seriesDeleted = false;
    auto episodesDeleted = false;
    auto bannersDeleted = false;
    
    this->startTransaction();
    
    QSqlQuery query(m_db);
    seriesDeleted = query.exec("DELETE FROM Series");
    if (seriesDeleted)
    {
        query.exec("DELETE FROM Episode");
        if (seriesDeleted && episodesDeleted)
        {
            bannersDeleted = query.exec("DELETE FROM Banner");
        }
    }
     
    auto result = seriesDeleted && episodesDeleted && bannersDeleted;
    
    return result ? this->commit() : this->rollback(), result;
}

bool DatabaseManager::checkIfAdded(const int &seriesId, const QString &name)
{
    qDebug() << seriesId << name;
    auto isAdded = false;
    
    QSqlQuery query(m_db);
    query.prepare("SELECT seriesName FROM Series WHERE id = :seriesId");
    query.bindValue(":seriesId", seriesId);
    query.exec();

    qDebug() << query.lastError();

    if (query.isSelect())
    {
        while (query.next())
        {
            if (query.value(0).toString() == name)
            {
                isAdded = true;
            }
        }
    }

    // emit checkIfAddedReady(seriesId, isAdded);
    return isAdded;
}

int DatabaseManager::watchedCount(int seriesId)
{
    auto watchedCount = 0;

    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(episodeName) "
                  "FROM Episode "
                  "WHERE seriesID = :seriesId AND watched = 1");
    query.bindValue(":seriesId", seriesId);
    query.exec();

    qDebug() << query.lastError();

    if (query.isSelect())
    {
        while (query.next())
        {
            watchedCount = query.value(0).toInt();
        }
    }

    return watchedCount;
}

int DatabaseManager::watchedCountBySeason(int seriesId, int seasonNumber)
{
    auto watchedCount = 0;

    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) "
                  "FROM Episode "
                  "WHERE seriesID = :seriesId AND watched = 1 AND seasonNumber = :seasonNumber");
    query.bindValue(":seriesId", seriesId);
    query.bindValue(":seasonNumber", seasonNumber);
    query.exec();

    qDebug() << query.lastError();

    if (query.isSelect())
    {
        while (query.next())
        {
            watchedCount = query.value(0).toInt();
        }
    }

    return watchedCount;
}

int DatabaseManager::totalCount(int seriesId)
{
    auto totalCount = 0;

    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(episodeName) "
                  "FROM Episode "
                  "WHERE seriesID = :seriesId");
    query.bindValue(":seriesId", seriesId);
    query.exec();

    qDebug() << query.lastError();

    if (query.isSelect())
    {
        while (query.next())
        {
            totalCount = query.value(0).toInt();
        }
    }

    return totalCount;
}

int DatabaseManager::totalCountBySeason(int seriesId, int seasonNumber)
{
    auto totalCount = 0;

    QSqlQuery query(m_db);
    query.prepare("SELECT COUNT(*) "
                  "FROM Episode "
                  "WHERE seriesID = :seriesId AND seasonNumber = :seasonNumber");
    query.bindValue(":seriesId", seriesId);
    query.bindValue(":seasonNumber", seasonNumber);
    query.exec();

    qDebug() << query.lastError();

    if (query.isSelect())
    {
        while (query.next())
        {
            totalCount = query.value(0).toInt();
        }
    }

    return totalCount;
}

int DatabaseManager::seasonCount(int seriesId)
{
    auto seasonCount = 0;

    QSqlQuery query(m_db);
    query.prepare("SELECT MAX(seasonNumber) "
                  "FROM Episode "
                  "WHERE seriesID = :seriesId");
    query.bindValue(":seriesId", seriesId);
    query.exec();

    qDebug() << query.lastError();

    if (query.isSelect())
    {
        while (query.next())
        {
            seasonCount = query.value(0).toInt();
        }
    }

    return seasonCount;
}

void DatabaseManager::markSeasonAsWatched(const int &seriesId, const int &seasonNumber)
{
    QSqlQuery query(m_db);
    query.prepare("UPDATE Episode "
                  "SET watched = 1 "
                  "WHERE seriesID = :seriesId AND seasonNumber = :seasonNumber");
    query.bindValue(":seriesId", seriesId);
    query.bindValue(":seasonNumber", seasonNumber);
    query.exec();

    qDebug() << query.lastError();

    getEpisodes(seriesId, seasonNumber);
    getSeasons(seriesId);
}

QVariantMap DatabaseManager::getNextEpisodeDetails(const int &seriesId) const
{
    const auto today = QDateTime::currentDateTime().date().toString(Qt::ISODate);
    
    QList<QVariantMap> episodes;
    
    QSqlQuery query(m_db);
    query.prepare("SELECT episodeName, episodeNumber, seasonNumber, firstAired "
                  "FROM Episode "
                  "WHERE seriesID = :seriesId AND firstAired >= :today "
                  "ORDER BY episodeNumber "
                  "LIMIT 1");
    query.bindValue(":seriesId", seriesId);
    query.bindValue(":today", today);
    query.exec();

    qDebug() << query.lastError();
    
    if (query.isSelect())
    {
        while (query.next())
        {
            QVariantMap episode;
            
            auto episodeName = query.value(0).toString();
            episodeName.replace("''", "'");
            episode["nextEpisodeName"] = episodeName;
            
            const auto episodeNumber = query.value(1).toString();
            episode["nextEpisodeNumber"] = episodeNumber;
            
            const auto seasonNumber = query.value(2).toString();
            episode["nextEpisodeSeasonNumber"] = seasonNumber;
            
            const auto firstAired = query.value(3).toString();
            episode["nextEpisodeFirstAired"] = firstAired;
            
            episodes.append(episode);
        }
    }
    
    QVariantMap nextEpisodeDetails;
    
    if (!episodes.isEmpty())
    {
        nextEpisodeDetails = episodes.first();
        
        const auto firstAired = nextEpisodeDetails["nextEpisodeFirstAired"].toString();
        const auto daysToNextEpisode = QDate::fromString(today, "yyyy-MM-dd").daysTo(QDate::fromString(firstAired, "yyyy-MM-dd"));
        
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

QString DatabaseManager::getStatus(const int &seriesId) const
{
    QString status;

    QSqlQuery query(m_db);
    query.prepare("SELECT status FROM Series WHERE id = :id");
    query.bindValue(":id", seriesId);
    query.exec();

    qDebug() << query.lastError();

    if (query.isSelect())
    {
        while (query.next())
        {
            status = query.value(0).toString();
        }
    }

    return status;
}

QString DatabaseManager::getSeasonBanner(const int &seriesId, const int &season) const
{
    QString banner = "";
    QString bannerType = "season";

    QSqlQuery query(m_db);
    query.prepare("SELECT bannerPath "
                  "FROM Banner "
                  "WHERE seriesID = :seriesId AND bannerType = :bannerType AND season = :season");
    query.bindValue(":seriesId", seriesId);
    query.bindValue(":bannerType", bannerType);
    query.bindValue(":season", season);
    query.exec();

    qDebug() << query.lastError();

    if (query.isSelect())
    {
        while (query.next())
        {
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
               "LEFT JOIN episode ON episode.seriesID = series.id AND episode.watched = 1");

    qDebug() << query.lastError();

    if (query.isSelect())
    {
        while (query.next())
        {
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
               "LEFT JOIN episode ON episode.seriesID = series.id AND episode.watched = 1");

    qDebug() << query.lastError();

    if (query.isSelect())
    {
        while (query.next())
        {
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
               "WHERE episode.watched = 1");

    qDebug() << query.lastError();

    if (query.isSelect())
    {
        while (query.next())
        {
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
               "FROM episode");

    qDebug() << query.lastError();

    if (query.isSelect())
    {
        while (query.next())
        {
            count = query.value(0).toInt();
        }
    }

    return count;
}

int DatabaseManager::getAllSeriesCount()
{
    auto count = 0;

    QSqlQuery query(m_db);
    query.exec("SELECT COUNT(*) FROM series");

    qDebug() << query.lastError();

    if (query.isSelect())
    {
        while (query.next())
        {
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
               "WHERE series.status = 'Ended'");

    qDebug() << query.lastError();

    if (query.isSelect())
    {
        while (query.next())
        {
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
               "WHERE episode.seriesID = series.id AND episode.watched = 0)");

    qDebug() << query.lastError();

    if (query.isSelect())
    {
        while (query.next())
        {
            inProgressSeriesCount = query.value(0).toInt();
        }
    }

    query.exec("SELECT COUNT(id) FROM series");

    qDebug() << query.lastError();

    if (query.isSelect())
    {
        while (query.next())
        {
            allSeriesCount = query.value(0).toInt();
        }
    }

    return allSeriesCount - inProgressSeriesCount;
}

void DatabaseManager::getStatistics()
{
    QVariantMap statistics;
    statistics["watchedEpisodesDuration"] = getWatchedEpisodesDuration();
    statistics["watchedEpisodesCount"] = getWatchedEpisodesCount();
    statistics["allEpisodesCount"] = getAllEpisodesCount();
    statistics["allSeriesCount"] = getAllSeriesCount();
    statistics["watchedSeriesCount"] = getWatchedSeriesCount();
    statistics["allSeasonsCount"] = getAllSeasonsCount();
    statistics["watchedSeasonsCount"] = getWatchedSeasonsCount();
    statistics["averageWatchedEpisodesDuration"] = getAverageWatchedEpisodesDuration();

    emit updateStatistics(statistics);
}

int DatabaseManager::getWatchedSeasonsCount()
{
    auto count = 0;

    QSqlQuery query(m_db);
    query.exec("SELECT * "
               "FROM (SELECT MAX(episodeNumber) AS episodes, SUM(watched) AS watched, seasonID FROM episode GROUP BY seasonID) "
               "GROUP BY seasonID "
               "HAVING episodes = watched");

    qDebug() << query.lastError();

    if (query.isSelect())
    {
        while (query.next())
        {
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
               "FROM (SELECT MAX(seasonNumber) AS seasonCount FROM episode GROUP BY seriesID)");

    qDebug() << query.lastError();

    if (query.isSelect())
    {
        while (query.next())
        {
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
               "ON episode.seriesID = series.id AND episode.watched = 1");

    if (query.isSelect())
    {
        while (query.next())
        {
            auto directorString = query.value(0).toString();
            auto seriesName = query.value(1).toString();

            if (directorString.contains("|"))
            {
                auto parsed = directorString.split("|");
                parsed.removeAll("");
                for (auto director : parsed)
                {
                    QMap<QString, QString> temp;
                    temp.insert(director, seriesName);
                    directors.append(temp);
                }
            }
            else
            {
                QMap<QString, QString> temp;
                temp.insert(directorString, seriesName);
                directors.append(temp);
            }
        }
    }

    QMap<QString, QStringList> occurences;
    for (auto director : directors)
    {
        occurences[director.firstKey()].append(director.first());
    }

    QMultiMap<int, QMap<QString, QStringList> > result;
    auto itr = occurences.begin();
    while (itr != occurences.end())
    {
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
    query.exec("SELECT actors, seriesName FROM series");

    if (query.isSelect())
    {
        while (query.next())
        {
            auto actor = query.value(0).toString();
            auto seriesName = query.value(1).toString();

            auto parsed = actor.split("|");
            parsed.removeAll("");
            for (auto actor : parsed)
            {
                QMultiMap<QString, QString> temp;
                temp.insert(actor, seriesName);
                actors.append(temp);
            }
        }
    }

    QMap<QString, QStringList> occurences;
    for (auto item : actors)
    {
        occurences[item.firstKey()].append(item.first());
    }

    QMultiMap<int, QMap<QString, QStringList> > result;
    auto itr = occurences.begin();
    while (itr != occurences.end())
    {
        auto size = itr.value().size();
        QMap<QString, QStringList> temp;
        temp.insert(itr.key(), itr.value());
        result.insert(size, temp);
        ++itr;
    }

    qDebug() << result;
    return result;
}
