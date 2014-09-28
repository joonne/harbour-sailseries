#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <QObject>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlError>
#include <QFile>
#include <QDir>
#include <QtSql/QSqlQuery>
#include <QTextStream>
#include <QStandardPaths>
#include <QVariant>
#include <QDebug>
#include "xmlreader.h"

class DatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit DatabaseManager(QObject *parent = 0);
    ~DatabaseManager();

    void setUpDB();

    bool openDB();
    bool deleteDB();
    QSqlError lastError();
    void close();
    bool createDB();
    bool updateDB();

    bool initializeInfoTable();
    bool createInfoTable();
    bool updateInfoTable(double version);

    bool createSeriesTable();
    bool createEpisodeTable();

signals:

public slots:

private:
    QSqlDatabase db;
    XMLReader* myReader;


};

#endif // DATABASEMANAGER_H
