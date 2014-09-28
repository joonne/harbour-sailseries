#ifndef SERIESDATA_H
#define SERIESDATA_H

#include <QObject>
#include <QPixmap>
#include <QDebug>

class SeriesData : public QObject
{
    Q_OBJECT
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
    explicit SeriesData(QObject *parent = 0);
    SeriesData(QObject* parent, QString ID, QString language, QString seriesName,
               QString aliasNames, QString banner, QString overview, QString firstAired, QString IMDB_ID,
               QString zap2it_ID, QString network);

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

public slots:

private:

    QString myID;
    QString myLanguage;
    QString mySeriesName;
    QString myAliasNames;
    QString myBanner;
    QString myOverview;
    QString myFirstAired;
    QString myIMDB_ID;
    QString myZap2it_ID;
    QString myNetwork;

};

#endif // SERIESDATA_H
