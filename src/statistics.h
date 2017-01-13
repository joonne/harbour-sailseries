#ifndef STATISTICS_H
#define STATISTICS_H

#include <QObject>

#include "databasemanager.h"

class Statistics : public QObject
{
    Q_OBJECT
    Q_PROPERTY(type name READ name WRITE setname NOTIFY nameChanged)
public:
    explicit Statistics(QObject *parent = 0);

signals:

public slots:

private:
    DatabaseManager* m_dbmanager;
};

#endif // STATISTICS_H
