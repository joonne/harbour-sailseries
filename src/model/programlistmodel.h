#ifndef PROGRAMLISTMODEL_H
#define PROGRAMLISTMODEL_H

#include <QObject>
#include <QQmlListProperty>
#include <QMap>
#include <QAbstractListModel>

#include "programdata.h"
#include "../databasemanager.h"
#include "../xmlreader.h"

class ProgramListModel : public QAbstractListModel
{
    Q_OBJECT
    //Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
public:
    enum ProgramListRoles {
        ChannelNameRole,
        ProgramNameRole,
        TimeRole,
        WeekdayRole,
        OverviewRole
    };

    explicit ProgramListModel(QObject *parent = 0, DatabaseManager* dbmanager = 0, XMLReader* reader = 0);
    ~ProgramListModel();

    QVariant data(const QModelIndex& index, int role) const;
    int rowCount(const QModelIndex& index = QModelIndex()) const;
    QVariantMap get(const QModelIndex& index) const;
    void setProgramList(QList<ProgramData> programs);
    void addProgram(ProgramData* program);
    void clear();
    void populateChannel();

signals:
    void countChanged();

protected:
    QHash<int,QByteArray> roleNames();

private:
    QList<ProgramData*> m_programs;
    DatabaseManager* m_dbmanager;
    XMLReader* m_reader;

};

Q_DECLARE_METATYPE(ProgramListModel*)

#endif // PROGRAMLISTMODEL_H
