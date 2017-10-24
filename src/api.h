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
    QList<QVariantMap> parseImages(QJsonObject obj);
    QList<QVariantMap> parseActors(QJsonObject obj);
    QList<QVariantMap> parseEpisodes(QJsonObject obj);

    QNetworkReply* get(QUrl url);

    void getAuthenticationToken();
    void getLanguages();
    void searchSeries(QString seriesName);
    void getSeries(QString seriesId);
    void getSeasonImages(QString seriesId);
    void getPosterImages(QString seriesId);
    void getSeriesImages(QString seriesId);
    void getFanartImages(QString seriesId);
    void getActors(QString seriesId);
    void getEpisodes(QString seriesId, int page);
    void getAll(QString seriesId, QString method);

    QList<QVariantMap> series();
    QList<QVariantMap> episodes();
    QList<QVariantMap> banners();

    bool getUpdateFlag();
    void setUpdateFlag(bool state);
    bool getFullRecordFlag();
    void setFullRecordFlag(bool state);

    QString findHighestRatedImage(QList<QVariantMap> images);

signals:
    void readyToGetBanners();
    void readyToPopulateSeries();
    void readyToPopulateChannels();
    void readyToStoreSeries();
    void readyToUpdateSeries();
    void readyToCheckIfReady(bool episodesFinished = false);

public slots:
    void replyFinishedError(QNetworkReply* reply);
    void checkIfReady(bool episodesFinished);

private:
    QNetworkAccessManager* m_nam;
    QList<QMap<QString,QString> > m_languages;
    QList<QVariantMap> m_series;
    QList<QVariantMap> m_episodes;
    QList<QVariantMap> m_actors;
    QList<QVariantMap> m_fanartImages;
    QList<QVariantMap> m_seasonImages;
    QList<QVariantMap> m_seriesImages;
    QList<QVariantMap> m_posterImages;
    QString m_currentServerTime;
    bool m_fullRecord;
    bool m_update;
    QString m_jwt;

    QString getLocale();
};

#endif // API_H
