#ifndef CHANNELMODEL_H
#define CHANNELMODEL_H

#include <QAbstractListModel>
#include <QVariant>
#include "programdata.h"

class ChannelModel : public QAbstractListModel
{
    Q_OBJECT
public:

    enum ProgramListRoles {
        ChannelNameRole,
        ProgramNameRole,
        TimeRole,
        WeekdayRole,
        OverviewRole
    };

    explicit ChannelModel(QObject *parent = 0);
    //~ChannelModel();

//    int QAbstractItemModel::rowCount ( const QModelIndex & parent = QModelIndex() ) const;
//    QVariant QAbstractItemModel::data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
//    QVariant QAbstractItemModel::headerData ( int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;

signals:

public slots:

};

#endif // CHANNELMODEL_H
