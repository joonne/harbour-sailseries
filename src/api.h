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
    QList<QVariantMap> parseJSON(QJsonObject obj);
    QVariantMap parseEpisode(QJsonObject obj);

    QNetworkReply* get(QUrl url);

    void getAuthenticationToken();
    void getLanguages();
    void searchSeries(QString seriesName);
    void getSeries(QString seriesId);
    void getSeasonImages(QString seriesId);
    void getPosterImages(QString seriesId);
    void getBannerImages(QString seriesId);
    void getFanartImages(QString seriesId);
    void getActors(QString seriesId);
    void getEpisodes(QString seriesId, int page = 1);
    void getAll(QString seriesId);
    void getEpisode(QString episodeId);

signals:
    void readyToPopulateSeries(QList<QVariantMap>);
    void storeSeries(QVariantMap);
    void readyToPopulateEpisodeDetails(QVariantMap episode);
    void storeEpisodes(QString, QList<QVariantMap>);
    void storePosterImageFor(QString, QString);
    void storeBannerImageFor(QString, QString);
    void storeFanartImageFor(QString, QString);
    void storeActors(QString, QList<QVariantMap>);
    void storeSeasonImages(QString, QList<QVariantMap>);

public slots:
    void replyFinishedError(QNetworkReply* reply);

private:
    QNetworkAccessManager* m_nam;
    QList<QMap<QString,QString> > m_languages;
    QString m_jwt;

    QString getLocale();
    QString findHighestRatedImage(QList<QVariantMap> images);
};

#endif // API_H
