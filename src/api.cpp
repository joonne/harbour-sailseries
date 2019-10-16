#include "api.h"

#define APIKEY "88D0BD893851FA78"
#define MIRRORPATH "https://api.thetvdb.com"

Api::Api(QObject *parent) :
    QObject(parent),
    m_nam(new QNetworkAccessManager),
    m_jwt("")
{
    getAuthenticationToken();
}

Api::~Api()
{
    delete m_nam;
    m_nam = 0;
}

QNetworkReply* Api::get(QUrl url)
{
    QNetworkRequest request(url);

    request.setRawHeader(QByteArray("Content-Type"), QByteArray("application/json"));
    request.setRawHeader(QByteArray("Authorization"), (QString("Bearer %1").arg(m_jwt)).toLocal8Bit());
    request.setRawHeader(QByteArray("Accept-Language"), QByteArray("en"));

    qDebug() << "REQUESTING" << request.url().toString();

    return m_nam->get(request);
}

void Api::getAuthenticationToken()
{
    QUrl url(QString("%1/login").arg(QString(MIRRORPATH)));
    QNetworkRequest request(url);

    request.setRawHeader("Content-Type", "application/json");

    QByteArray body = "{\"apikey\": \"88D0BD893851FA78\"}";

    QNetworkReply* reply = m_nam->post(request, body);

    connect(reply, &QNetworkReply::finished, [this, reply]()
    {
        auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

        if (!jsonDocument.isNull())
        {
            m_jwt = jsonDocument.object().value("token").toString();
        }

        reply->deleteLater();
    });
}

QString Api::getLocale()
{
    auto systemLocale = QLocale::system().name();
    QString locale = "en";

    for (auto language : m_languages) {
        if (language["abbreviation"] == systemLocale) {
            locale = language["abbreviation"];
        }
    }

    return locale;
}

void Api::getLanguages()
{
    QUrl url(QString("%1/languages").arg(QString(MIRRORPATH)));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply]()
    {
       auto document = QJsonDocument::fromJson(reply->readAll());

       if (!document.isNull()) {
           auto jsonArray = document.object().value("data").toArray();
           qDebug() << jsonArray;
       }

       reply->deleteLater();
    });
}

void Api::searchSeries(QString text)
{
    QUrl url(QString("%1/search/series?name=%2").arg(QString(MIRRORPATH)).arg(text));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply]()
    {
        auto document = QJsonDocument::fromJson(reply->readAll());

        if (!document.isNull())
        {
            auto series = parseSeries(document.object());
            emit(readyToPopulateSeries(series));
        }

        reply->deleteLater();
    });
}

void Api::getAll(QString seriesId)
{
    getSeries(seriesId);
    getEpisodes(seriesId);
    getActors(seriesId);
    getSeasonImages(seriesId);
    getPosterImages(seriesId);
    getBannerImages(seriesId);
    getFanartImages(seriesId);
}

void Api::getSeries(QString seriesId)
{
    QUrl url(QString("%1/series/%2").arg(QString(MIRRORPATH)).arg(seriesId));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull())
       {
           auto series = parseSeries(jsonDocument.object());
           emit storeSeries(series.first());
       }

       reply->deleteLater();
    });
}

void Api::getSeasonImages(QString seriesId)
{
    QUrl url(QString("%1/series/%2/images/query?keyType=season").arg(QString(MIRRORPATH)).arg(seriesId));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull())
       {
           auto seasonImages = parseJSON(jsonDocument.object());

           std::transform(seasonImages.begin(), seasonImages.end(), seasonImages.begin(), [seriesId](QVariantMap seasonImage)
           {
               QVariantMap image;
               image.insert("id", seasonImage["id"]);
               image.insert("seriesId", seriesId);
               image.insert("bannerPath", seasonImage["fileName"]);
               image.insert("bannerType", seasonImage["keyType"]);
               image.insert("bannerType2", "");
               image.insert("language", seasonImage["id"]);
               image.insert("season", seasonImage["subKey"]);

               return image;
           });

           emit storeSeasonImages(seriesId, seasonImages);
       }

       reply->deleteLater();
    });
}

void Api::getPosterImages(QString seriesId)
{
    QUrl url(QString("%1/series/%2/images/query?keyType=poster").arg(QString(MIRRORPATH)).arg(seriesId));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull())
       {
           auto posters = parseJSON(jsonDocument.object());
           emit storePosterImageFor(seriesId, findHighestRatedImage(posters));
       }

       reply->deleteLater();
    });
}

void Api::getBannerImages(QString seriesId)
{
    QUrl url(QString("%1/series/%2/images/query?keyType=series").arg(QString(MIRRORPATH)).arg(seriesId));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull())
       {
           auto banners = parseJSON(jsonDocument.object());
           emit storeBannerImageFor(seriesId, findHighestRatedImage(banners));
       }

       reply->deleteLater();
    });
}

void Api::getFanartImages(QString seriesId)
{
    QUrl url(QString("%1/series/%2/images/query?keyType=fanart").arg(QString(MIRRORPATH)).arg(seriesId));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull())
       {
           auto fanartImages = parseJSON(jsonDocument.object());
           emit storeFanartImageFor(seriesId, findHighestRatedImage(fanartImages));
       }

       reply->deleteLater();
    });
}

void Api::getActors(QString seriesId)
{
    QUrl url(QString("%1/series/%2/actors").arg(QString(MIRRORPATH)).arg(seriesId));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull())
       {
           auto actors = parseJSON(jsonDocument.object());
           emit storeActors(seriesId, actors);
       }

       reply->deleteLater();
    });
}

void Api::getEpisodes(QString seriesId, int page)
{
    QUrl url(QString("%1/series/%2/episodes?page=%3").arg(QString(MIRRORPATH)).arg(seriesId).arg(page));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId, page]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull())
       {
           if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 404)
           {
               qDebug() << "all episodes for " << seriesId << " fetched";
               return;
           }

           qDebug() << "store episodes for " << seriesId;
           emit storeEpisodes(seriesId, parseJSON(jsonDocument.object()));

           qDebug() << "try to get more episodes for " << seriesId;
           getEpisodes(seriesId, page + 1);
       }
    });
}

void Api::getEpisode(QString episodeId)
{
    QUrl url(QString("%1/episodes/%2").arg(QString(MIRRORPATH)).arg(episodeId));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull())
       {
           auto episode = parseEpisode(jsonDocument.object());
           emit readyToPopulateEpisodeDetails(episode);
       }
    });
}

QString Api::findHighestRatedImage(QList<QVariantMap> images)
{
    auto result = !images.isEmpty() ? images.first() : QVariantMap();

    for (auto image: images)
    {
        if (
                image.contains("ratingsInfo") &&
                image["ratingsInfo"].toMap().contains("average") &&
                result.contains("ratingsInfo") &&
                result["ratingsInfo"].toMap().contains("average") &&
                image["ratingsInfo"].toMap()["average"].toDouble() > result["ratingsInfo"].toMap()["average"].toDouble()
           )
        {
            result = image;
        }
    }

    return result["fileName"].toString();
}

// ---------------------------------------------------
// slots
// ---------------------------------------------------

void Api::replyFinishedError(QNetworkReply *reply)
{
    qDebug() << reply->url() << reply->errorString();
    
    QVariantMap temp;
    temp.insert("SeriesName", "Error, try again later.");

    QList<QVariantMap> series;
    series.append(temp);

    emit readyToPopulateSeries(series);

    reply->deleteLater();
}

// ---------------------------------------------------
// JSON handling
// ---------------------------------------------------

QList<QVariantMap> Api::parseSeries(QJsonObject obj)
{
    QList<QVariantMap> allSeries;

    QJsonArray foundSeries;

    if (obj.value("data").isArray()) {
        foundSeries = obj.value("data").toArray();
    } else if (obj.value("data").isObject()) {
        foundSeries.append(obj.value("data"));
    }

    for (auto item : foundSeries) {
        QVariantMap series;

        auto jsonObject = item.toObject();
        auto keys = jsonObject.keys();
        for (auto key : keys) {
            series.insert(key, jsonObject.value(key).toVariant());
        }

        allSeries.append(series);
    }

    return allSeries;
}

QVariantMap Api::parseEpisode(QJsonObject obj)
{
    QJsonObject jsonObject = obj.value("data").toObject();

    QVariantMap episode;

    auto keys = jsonObject.keys();
    for (auto key : keys)
    {
        episode.insert(key, jsonObject.value(key).toVariant());
    }

    return episode;
}

QList<QVariantMap> Api::parseJSON(QJsonObject obj)
{
    QList<QVariantMap> results;

    auto jsonArray = obj.value("data").toArray();

    for (auto item : jsonArray)
    {
        QVariantMap result;

        auto jsonObject = item.toObject();
        auto keys = jsonObject.keys();
        for (auto key : keys)
        {
            result.insert(key, jsonObject.value(key).toVariant());
        }

        results.append(result);
    }

    return results;
}
