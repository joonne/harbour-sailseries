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
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

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
    void get(QUrl url);

    void getLanguages();
    void searchSeries(QString text);
    void getFullSeriesRecord(QString seriesid, QString method);

    void getAuthenticationToken();
    
    QList<QVariantMap> getSeries();
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
    void replyFinishedNew(QNetworkReply* reply);

private:
    QNetworkAccessManager* m_nam;
    QList<QMap<QString,QString> > m_languages;
    QList<QVariantMap> m_series;
    QList<QMap<QString,QString> > m_episodes;
    QList<QMap<QString,QString> > m_banners;
    QString m_currentServerTime;
    QString m_jwt;

    bool m_fullRecord;
    bool m_update;

    QString getLocale();    
    void getJwt();

    QString m_jwt;
};

#endif // XMLREADER_H
