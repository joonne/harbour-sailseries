#ifndef XMLREADER_H
#define XMLREADER_H

#include <QObject>
#include <QXmlStreamReader>
#include <QFile>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QTemporaryFile>
#include <QList>
#include <QStringRef>
#include <QByteArray>
#include <QBuffer>
#include "qzipreader_p.h"

class XMLReader : public QObject
{
    Q_OBJECT
public:

    explicit XMLReader(QObject *parent = 0);
    ~XMLReader();

    void parseXML(QXmlStreamReader &xml);
    QMap<QString,QString> parseSeries(QXmlStreamReader& xml);
    QMap<QString,QString> parseLanguages(QXmlStreamReader& xml);
    QMap<QString,QString> parseEpisode(QXmlStreamReader& xml);
    QString parseServerTime(QXmlStreamReader& xml);
    void addElementDataToMap(QXmlStreamReader& xml,
                                 QMap<QString, QString>& map) const;
    void startRequest(QUrl url);

    void getLanguages();
    void searchSeries(QString text);
    void getServerTime();
    void getFullSeriesRecord(QString seriesid, QString method);
    void getBanners(QString seriesid);

    QList<QMap<QString,QString> > getSeries();
    QList<QMap<QString,QString> > getEpisodes();

signals:
    void readyToPopulateSeries();
    void readyToPopulateChannels();
    void readyToGetBanner();
    void readyToStoreSeries();
    void readyToUpdateSeries();

public slots:
    void replyFinished(QNetworkReply* reply);

private:

    QNetworkAccessManager* myNetWorkAccessManager;
    QList<QMap<QString,QString> > myLanguages;
    QList<QMap<QString,QString> > mySeries;
    QList<QMap<QString,QString> > myEpisodes;
    QString currentServerTime;

    bool fullRecord;
    bool update;

};

#endif // XMLREADER_H
