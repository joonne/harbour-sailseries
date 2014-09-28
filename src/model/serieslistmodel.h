#ifndef SERIESLISTMODEL_H
#define SERIESLISTMODEL_H

#include <QObject>
#include <QQmlContext>
#include "seriesdata.h"
#include "../xmlreader.h"
#include <QDebug>
#include <QVariantList>
#include <QStringList>
#include <QQmlListProperty>

class seriesListModel : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<SeriesData> seriesList READ getSeriesList NOTIFY seriesListChanged)
    Q_PROPERTY(QString ID READ getID)
    Q_PROPERTY(QString Language READ getLanguage)
    Q_PROPERTY(QString SeriesName READ getSeriesName)
    Q_PROPERTY(QString AliasNames READ getAliasNames)
    Q_PROPERTY(QString Banner READ getBanner)
    Q_PROPERTY(QString Overview READ getOverview)
    Q_PROPERTY(QString FirstAired READ getFirstAired)
    Q_PROPERTY(QString IMDB_ID READ getIMDB_ID)
    Q_PROPERTY(QString zap2it_ID READ getZap2it_ID)
    Q_PROPERTY(QString Network READ getNetwork)

public:
    explicit seriesListModel(QObject *parent = 0, QQmlContext* context = 0);
    ~seriesListModel();

    void populateSeriesList();
    Q_INVOKABLE void searchSeries(QString text);
    Q_INVOKABLE void selectSeries(int index);

    QQmlListProperty<SeriesData> getSeriesList();
    QString getID();
    QString getLanguage();
    QString getSeriesName();
    QString getAliasNames();
    QString getBanner();
    QString getOverview();
    QString getFirstAired();
    QString getIMDB_ID();
    QString getZap2it_ID();
    QString getNetwork();


signals:
    void seriesListChanged();

public slots:
    void xmlParseFinished();

private:
    XMLReader* myReader;
    QList<QMap<QString,QString> > mySeries;
    QQmlContext* myContext;
    QList<SeriesData*> mySeriesListModel;
    bool isPopulated;
    SeriesData* myInfo;

    static int seriesListCount(QQmlListProperty<SeriesData> *prop);
    static SeriesData* seriesListAt(QQmlListProperty<SeriesData> *prop, int index);
    static void seriesListAppend(QQmlListProperty<SeriesData>* prop, SeriesData* val);
    static void seriesListClear(QQmlListProperty<SeriesData>* prop);



};

#endif // SERIESLISTMODEL_H
