#ifndef API_H
#define API_H

#include <QObject>
#include <QDebug>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QList>
#include <QByteArray>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonValue>

class Api : public QObject
{
    Q_OBJECT
public:

    explicit Api(QObject *parent = 0);
    ~Api();

    QList<QVariantMap> parseSeries(QJsonObject obj);
    QVariantMap parseImages(QJsonObject obj);
    QVariantMap parseActors(QJsonObject obj);

    QNetworkReply* get(QUrl url);

    void getAuthenticationToken();
    void getLanguages();
    void searchSeries(QString seriesName);
    void getSeriesFromApi(QString seriesId, QString method);
    void getImages(QString seriesId);
    void getActors(QString seriesId);

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
    void replyFinishedError(QNetworkReply* reply);

private:
    QNetworkAccessManager* m_nam;
    QList<QMap<QString,QString> > m_languages;
    QList<QVariantMap> m_series;
    QList<QVariantMap> m_episodes;
    QList<QVariantMap> m_banners;
    QString m_currentServerTime;
    bool m_fullRecord;
    bool m_update;
    QString m_jwt;

    QString getLocale();
};

#endif // API_H
