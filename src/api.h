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
    void searchSeries(const QString &seriesName);
    void getSeries(const int &seriesId);
    void getSeasonImages(const int &seriesId);
    void getPosterImages(const int &seriesId);
    void getBannerImages(const int &seriesId);
    void getFanartImages(const int &seriesId);
    void getActors(const int &seriesId);
    void getEpisodes(const int &seriesId, const int &page = 1);
    void getAll(const int &seriesId);
    void getEpisode(const int &episodeId);

signals:
    void readyToPopulateSeries(QList<QVariantMap>);
    void storeSeries(QVariantMap);
    void readyToPopulateEpisodeDetails(QVariantMap episode);
    void storeEpisodes(const int&, const QList<QVariantMap>&);
    void storePosterImageFor(const int&, const QString&);
    void storeBannerImageFor(const int&, const QString&);
    void storeFanartImageFor(const int&, const QString&);
    void storeActors(const int&, const QList<QVariantMap>&);
    void storeSeasonImages(const int&, const QList<QVariantMap>&);

public slots:
    void replyFinishedError(QNetworkReply* reply);

private:
    QNetworkAccessManager* m_nam;
    QList<QMap<QString,QString> > m_languages;
    QString m_jwt;

    QString getLocale();
    QString findHighestRatedImage(const QList<QVariantMap> images);
};

#endif // API_H
