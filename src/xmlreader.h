#ifndef XMLREADER_H
#define XMLREADER_H

#include <QObject>
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

    QList<QVariantMap> parseSeriesNew(QJsonObject obj);
    QVariantMap parseImages(QJsonObject obj);

    void get(QUrl url);

    void getLanguages();
    void searchSeries(QString seriesName);
    void getFullSeriesRecord(QString seriesId, QString method);
    void getFullSeriesRecordNew(QString seriesId, QString method);

    void getAuthenticationToken();

    QList<QVariantMap> getSeries();
    QList<QVariantMap> getEpisodes();
    QList<QVariantMap> getBanners();

    bool getUpdateFlag();
    void setUpdateFlag(bool state);
    bool getFullRecordFlag();
    void setFullRecordFlag(bool state);

signals:
    void readyToGetBanners();
    void readyToPopulateSeries();
    void readyToPopulateChannels();
    void readyToStoreSeries();
    void readyToUpdateSeries();

public slots:
    void replyFinished(QNetworkReply* reply);
    void replyFinishedNew(QNetworkReply* reply);
    void getSeriesFinished(QString seriesId, QString method);

private:
    QNetworkAccessManager* m_nam;
    QList<QMap<QString,QString> > m_languages;
    QList<QVariantMap> m_series;
    QList<QVariantMap> m_episodes;
    QList<QVariantMap> m_banners;
    QString m_currentServerTime;

    bool m_fullRecord;
    bool m_update;

    QString getLocale();    
    void getJwt();

    QString m_jwt;
};

#endif // XMLREADER_H
