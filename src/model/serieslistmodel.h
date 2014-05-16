#ifndef SERIESLISTMODEL_H
#define SERIESLISTMODEL_H

#include <QObject>
#include <QQmlListProperty>
#include "seriesdata.h"
#include "../xmlreader.h"
#include <QDebug>

/**
 * Private class to hold members for the actual SampleListModel.
 */
class SeriesListModelPrivate
{
public:
    // holds list property
    QQmlListProperty<SeriesData>* seriesList;

    // holds SampleData pointers (the contents for the list)
    QList<SeriesData*> seriesListData;

    // a flag that tells if the model has been populated
    // using populateSeriesList() method
    bool isPopulated;
};

class SeriesListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<SeriesData> seriesList READ seriesList NOTIFY seriesListChanged)

public:
    explicit SeriesListModel(QObject *parent = 0);
    ~SeriesListModel();

    QQmlListProperty<SeriesData> seriesList();

    void populateSeriesList();
    Q_INVOKABLE void searchSeries(QString text);

public slots:
    void xmlParseFinished();

signals:
    void seriesListChanged();

private:
    SeriesListModelPrivate* mySeriesModel;
    XMLReader* myReader;
    QList<QMap<QString,QString> > mySeries;

};

#endif // SERIESLISTMODEL_H
