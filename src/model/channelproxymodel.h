#ifndef CHANNELPROXYMODEL_H
#define CHANNELPROXYMODEL_H

#include <QAbstractProxyModel>


class ChannelProxyModel : public QAbstractProxyModel
{
    Q_OBJECT
public:

    //QModelIndex QAbstractProxyModel::mapFromSource ( const QModelIndex & sourceIndex ) const;
    //QModelIndex QAbstractProxyModel::mapToSource ( const QModelIndex & proxyIndex ) const;

    explicit ChannelProxyModel(QObject *parent = 0);
    //~ChannelProxyModel();

signals:

public slots:

};

#endif // CHANNELPROXYMODEL_H
