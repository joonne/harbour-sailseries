#include "databasemanager.h"

DatabaseManager::DatabaseManager(QObject *parent) :
    QObject(parent)
{
}

DatabaseManager::~DatabaseManager() {

}

void DatabaseManager::setUpDB() {

    if(openDB()) {

        QSqlQuery query(db);
        query.exec("SELECT count(*) FROM sqlite_master WHERE type = 'table';");
        int tableCount = query.value(1).toInt();
        qDebug() << "Tables in database: " << tableCount;

        if(tableCount == 0) {

            createDB();

        } else {

            QSqlQuery query(db);
            query.exec("SELECT version FROM information;");
            double version = query.value(1).toDouble();

            qDebug() << "Database already created, current version is: " << version;

            updateDB();
        }
    }

    close();

}

bool DatabaseManager::openDB() {

    QString dbname = "sailSeries.db.sqlite";
    QString dbpath = QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + dbname;
    QDir path(QStandardPaths::writableLocation(QStandardPaths::DataLocation) + QDir::separator() + dbname);

    if(!path.exists()) {

        path.mkpath(dbpath);

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

    if(db.isOpen()) {

        //-----------------------------------------------------------------

        if(initializeInfoTable()) qDebug() << "information table created";

        //-----------------------------------------------------------------

        if(true) {

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
                                 "(id INTEGER PRIMARY KEY NOT NULL , "
                                 "actors VARCHAR(1000), "
                                 "airsDayOfWeek VARCHAR(15), "
                                 "airsTime VARCHAR(10), "
                                 "contentRating VARCHAR(5), "
                                 "firstAired VARCHAR(10), "
                                 "genre VARCHAR(100), "
                                 "imdbID integer, "
                                 "language VARCHAR(20), "
                                 "network VARCHAR(2) NOT NULL, "
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
                                 "posters VARCHAR(50), "
                                 "zap2itID INTEGER)"));
    }
    return ret;
}
