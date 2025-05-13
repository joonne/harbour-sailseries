#include "databasemanager.h"

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{
    if (open()) {
       migrate();
    } else {
        qInfo() << "Database could not be opened";
    }
}

DatabaseManager::~DatabaseManager()
{
    close();
}

DatabaseManager::DbVersion DatabaseManager::getVersion() {
    DbVersion result = { .number = -1, .appName = "" };

    if (m_db.isOpen())
    {
        QSqlQuery query(m_db);
        query.exec("SELECT name, version FROM information");
        qDebug() << query.lastError();

        if (query.isSelect())
        {
            while (query.next())
            {
                auto name = query.value(0).toString();
                auto version = query.value(1).toInt();
                result = { .number = version, .appName = name };
                return result;
            }
        }
    }

    return result;
}

void DatabaseManager::migrate()
{
    if (m_db.isOpen())
    {
        if (m_db.tables().size() == 0)
        {
            if (createInfoTable())
            {
                qInfo() << "Information table created";
            }
        }

        auto version = getVersion();

        qInfo() << "App name: " << version.appName;
        qInfo() << "Database version: " << version.number;

        QList<Migration> migrations = {
            Migration {
                .version = 0,
                .operations = {
                    [this](){ createSeriesTable(); },
                    [this](){ createEpisodeTable(); },
                    [this](){ updateInfoTable(1); }
                }
            },
            Migration {
                .version = 1,
                .operations = {
                    [this](){ createBannerTable(); },
                    [this]() { updateInfoTable(2); }
                }
            },
            Migration {
                .version = 2,
                .operations = {
                    [this]() { addRuntimeToEpisode(); },
                    [this]() { updateInfoTable(3); }
                }
            }
        };

        for (auto migration: migrations) {
            if (migration.version >= version.number) {
                qInfo() << "Applying migration: " << migration.version;
                for (auto operation: migration.operations) {
                    transaction();
                    operation();
                    commit();
                }
            }
        }
    }
}

bool DatabaseManager::open()
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

void DatabaseManager::close()
{
    m_db.close();
}

bool DatabaseManager::transaction()
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

bool DatabaseManager::createInfoTable()
{
    bool isCreated = false;
    bool isInitted = false;
    
    if (m_db.isOpen())
    {
        QSqlQuery query(m_db);
        isCreated = query.exec("CREATE TABLE information"
                         "(version REAL DEFAULT 0 PRIMARY KEY, "
                         "name VARCHAR(50) DEFAULT SailSeries)");
        qDebug() << query.lastError();

        isInitted = query.exec("INSERT INTO information VALUES(0, 'SailSeries');");
        qDebug()<< query.lastError();
    }

    return isCreated && isInitted;
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
        qDebug() << query.lastError();
    }

    return result;
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
        qDebug() << query.lastError();
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
        qDebug() << query.lastError();
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
        qDebug() << query.lastError();
    }

    return result;
}

bool DatabaseManager::addRuntimeToEpisode()
{
    auto result = false;

    if (m_db.isOpen())
    {
        QSqlQuery query(m_db);
        result = query.exec("ALTER TABLE Episode ADD COLUMN runtime INTEGER;");
        qDebug() << query.lastError();
    }

    return result;
}

void DatabaseManager::storeSeries(const QVariantMap &series)
{
    auto added = series["added"].toString(); // migrate away
    auto addedBy = series["addedBy"].toInt(); // migrate away
    auto actors = series["actors"].toString(); // needed
    auto airsDayOfWeek = series["airsDays"].toString(); // needed
    auto airsTime = series["airsTime"].toString(); // needed
    auto banner = series["banner"].toString(); // => artworks
    auto contentRating = series["contentRating"].toString(); // ??
    auto fanart = series["fanart"].toString(); // nicetohave
    auto firstAired = series["firstAired"].toString(); // needed
    auto genres = series["genres"].toString(); // needed
    auto id = series["id"].toInt(); // needed
    auto remoteIds = series["remoteIds"].toMap();
    auto imdbId = remoteIds["IMDB"].toString(); // needed
    auto zap2itId = remoteIds["Zap2It"].toString(); // migrate away
    auto language = series["language"].toString(); // needed
    auto lastUpdated = series["lastUpdated"].toString();
    auto network = series["network"].toString(); // needed
    auto overview = series["overview"].toString(); // needed
    auto poster = series["poster"].toString(); // needed
    auto rating = series["score"].toDouble(); // needed
    auto ratingCount = series["siteRatingCount"].toInt(); // nicetohave
    auto runtime = series["runtime"].toInt(); // needed
    auto seriesName = series["name"].toString(); // needed
    auto status = series["status"].toString(); // needed
    auto watched = 0;
    
    overview.replace("'", "''");
    actors.replace("'", "''");
    seriesName.replace("'", "''");

    transaction();

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
        query.bindValue(":genre", genres);
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
    transaction();

    for (auto episode : episodes)
    {
        auto absoluteNumber = episode["number"].toInt();
        auto image = episode["image"].toString();
        auto episodeNumber = episode["number"].toInt();
        auto seasonNumber = episode["seasonNumber"].toInt();
        auto seasonId = episode["seasonId"].toInt();
        auto name = episode["name"].toString();
        auto firstAired = episode["aired"].toString();
        auto id = episode["id"].toInt();
        auto language = episode["language"].toMap()["overview"].toString();
        auto lastUpdated = episode["lastUpdated"].toString();
        auto overview = episode["overview"].toString();
        auto guestStars = episode["guestStars"].toStringList().join(", "); // need to find
        auto writers = episode["writers"].toStringList().join(", "); // need to find
        auto watched = 0;
        auto runtime = episode["runtime"].toInt();

        // important!
        overview.replace("'", "''");
        name.replace("'", "''");
        
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
            
            query.prepare("INSERT OR REPLACE INTO Episode (id, episodeName, episodeNumber, firstAired, guestStars, language, overview, seasonNumber, writer, absoluteNumber, filename, lastupdated, seasonID, seriesID, watched, runtime) "
                          "VALUES (:id, :episodeName, :episodeNumber, :firstAired, :guestStars, :language, :overview, :seasonNumber, :writer, :absoluteNumber, :filename, :lastUpdated, :seasonId, :seriesId, :watched, :runtime)");
            query.bindValue(":id", id);
            query.bindValue(":episodeName", name);
            query.bindValue(":episodeNumber", episodeNumber);
            query.bindValue(":firstAired", firstAired);
            query.bindValue(":guestStars", guestStars);
            query.bindValue(":language", language);
            query.bindValue(":overview", overview);
            query.bindValue(":seasonNumber", seasonNumber);
            query.bindValue(":writer", writers);
            query.bindValue(":absoluteNumber", absoluteNumber);
            query.bindValue(":filename", image);
            query.bindValue(":lastUpdated", lastUpdated);
            query.bindValue(":seasonId", seasonId);
            query.bindValue(":seriesId", seriesId);
            query.bindValue(":watched", watched);
            query.bindValue(":runtime", runtime);
            query.exec();

            qDebug() << query.lastError();
        }
    }

    commit();
}

void DatabaseManager::storeSeasonImages(const int &seriesId, const QList<QVariantMap> &banners)
{
    transaction();

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
        transaction();

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

void DatabaseManager::deleteSeries(const int &seriesId)
{   
    transaction();
    
    QSqlQuery query(m_db);
    query.prepare("DELETE FROM Series WHERE id = :seriesId");
    query.bindValue(":seriesId", seriesId);
    auto seriesDeleted = query.exec();

    query.prepare("DELETE FROM Episode WHERE seriesID = :seriesId");
    query.bindValue(":seriesId", seriesId);
    auto episodesDeleted = query.exec();

    query.prepare("DELETE FROM Banner WHERE seriesID = :seriesId");
    query.bindValue(":seriesId", seriesId);
    auto bannersDeleted = query.exec();

    if (seriesDeleted && episodesDeleted && bannersDeleted)
    {
        commit();
        emit this->seriesDeleted();
    }
    else
    {
        qDebug() << "Delete failed for " << seriesId;
        rollback();
    }
}

void DatabaseManager::getSeriesNames()
{
    QSqlQuery query(m_db);
    query.exec("SELECT seriesName FROM Series;");

    qDebug() << query.lastError();

    QSet<QString> seriesNames;

    if (query.isSelect())
    {
        while (query.next())
        {
            seriesNames.insert(query.value(0).toString());
        }
    }

    emit getSeriesNamesReady(seriesNames);
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
               "FROM episode "
               "WHERE episode.watched = 1");

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
               "FROM episode "
               "WHERE episode.watched = 1");

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
