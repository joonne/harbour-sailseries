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

signals:
    void readyToPopulateSeries(QList<QVariantMap>);
    void storeSeries(QVariantMap);
    void readyToPopulateEpisodeDetails(QVariantMap episode);
    void storeEpisodes(const int&, const QList<QVariantMap>&);
    void storeFanartImageFor(const int&, const QString&);
    void storeActors(const int&, const QList<QVariantMap>&);
    void storeSeasonImages(const int&, const QList<QVariantMap>&);
    void storeTranslations(const int&, const QVariantMap&);

public slots:
    void replyFinishedError(QNetworkReply* reply);
    void searchSeries(const QString &seriesName);
    void getAll(const int &seriesId);

private:
    QNetworkAccessManager* m_nam;
    QList<QMap<QString,QString> > m_languages;
    QString m_jwt;

    enum ARTWORK_TYPE {
        SERIES_BANNER = 1,
        SERIES_POSTER = 2,
        SEASON_BANNER = 6,
        SEASON_POSTER = 7,
    };

    void getAuthenticationToken();
    void getLanguages();
    void getSeries(const int &seriesId);
    void getTranslations(const int &seriesId, const QString &language);
    void getActors(const int &seriesId);
    void getEpisodes(const int &seriesId, const int &page = 0);
    QList<QVariantMap> parseJson(const QJsonObject &obj);
    QVariantMap parseSeries(const QJsonObject &obj);
    QVariant parseAirsDays(const QJsonObject &obj);
    QVariantMap parseRemoteIds(const QJsonArray &arr);
    QString parseGenres(const QJsonArray &arr);
    QList<QVariantMap> parseEpisodes(const QJsonArray &items);
    QList<QVariantMap> parseSearchResults(const QJsonArray &items);
    QList<QVariantMap> parseArtworks(const QJsonArray &items);
    QList<QVariantMap> parseSeasons(const QJsonObject &obj);
    QList<QVariantMap> toSeasonImages(const QList<QVariantMap> &seasons);
    QNetworkReply* get(QUrl url);
    QString getLocale();
    QString findHighestRatedImage(const QList<QVariantMap> &images, ARTWORK_TYPE type);
};

#endif // API_H
