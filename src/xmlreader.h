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
    QMap<QString,QString> parseBanner(QXmlStreamReader& xml);
    void addElementDataToMap(QXmlStreamReader& xml,
                                 QMap<QString, QString>& map) const;
    void startRequest(QUrl url);

    void getLanguages();
    void searchSeries(QString text);
    void getFullSeriesRecord(QString seriesid, QString method);

    QList<QMap<QString,QString> > getSeries();
    QList<QMap<QString,QString> > getEpisodes();
    QList<QMap<QString,QString> > getBanners();

    bool getUpdateFlag();
    void setUpdateFlag(bool state);
    bool getFullRecordFlag();
    void setFullRecordFlag(bool state);

signals:
    void readyToPopulateSeries();
    void readyToPopulateChannels();
    void readyToStoreSeries();
    void readyToUpdateSeries();

public slots:
    void replyFinished(QNetworkReply* reply);

private:

    QNetworkAccessManager* myNetWorkAccessManager;
    QList<QMap<QString,QString> > myLanguages;
    QList<QMap<QString,QString> > mySeries;
    QList<QMap<QString,QString> > myEpisodes;
    QList<QMap<QString,QString> > myBanners;
    QString currentServerTime;

    bool fullRecord;
    bool update;

    QString getLocale();

};

#endif // XMLREADER_H
