#include "api.h"

#define MIRRORPATH "https://api4.thetvdb.com/v4"

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
    request.setRawHeader(QByteArray("Authorization"), QByteArray::fromStdString(QString("Bearer %1").arg(m_jwt).toStdString()));
    request.setRawHeader(QByteArray("Accept-Language"), QByteArray("en"));

    qDebug() << "REQUESTING" << request.url().toString();

    return m_nam->get(request);
}

void Api::getAuthenticationToken()
{
    QUrl url(QString("%1/login").arg(QString(MIRRORPATH)));
    QNetworkRequest request(url);

    request.setRawHeader("Content-Type", "application/json");

    QByteArray body = QByteArray::fromStdString(QString("{\"apikey\": \"%1\"}").arg(QString(API_KEY)).toStdString());

    auto reply = m_nam->post(request, body);

    connect(reply, &QNetworkReply::finished, [this, reply]()
    {
        auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

        if (!jsonDocument.isNull())
        {
            m_jwt = jsonDocument.object().value("data").toObject().value("token").toString();
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

void Api::searchSeries(const QString &text)
{
    QUrl url(QString("%1/search?type=series&q=%2").arg(QString(MIRRORPATH)).arg(text));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply]()
    {
        auto document = QJsonDocument::fromJson(reply->readAll());

        if (!document.isNull())
        {
            auto series = parseJson(document.object());
            emit readyToPopulateSeries(series);
        }

        reply->deleteLater();
    });
}

void Api::getAll(const int &seriesId)
{
    getSeries(seriesId);
    getEpisodes(seriesId);
    getActors(seriesId);
    getSeasonImages(seriesId);
    getPosterImages(seriesId);
    getBannerImages(seriesId);
    getFanartImages(seriesId);
}

void Api::getSeries(const int &seriesId)
{
    QUrl url(QString("%1/series/%2").arg(QString(MIRRORPATH)).arg(seriesId));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull())
       {
           auto series = parseJson(jsonDocument.object()).first();
           emit storeSeries(series);
       }

       reply->deleteLater();
    });
}

void Api::getSeasonImages(const int &seriesId)
{
    QUrl url(QString("%1/series/%2/images/query?keyType=season").arg(QString(MIRRORPATH)).arg(seriesId));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull())
       {
           auto seasonImages = parseJson(jsonDocument.object());

           std::transform(seasonImages.begin(), seasonImages.end(), seasonImages.begin(), [seriesId](QVariantMap seasonImage)
           {
               QVariantMap image;
               image.insert("id", seasonImage["id"]);
               image.insert("seriesId", seriesId);
               image.insert("bannerPath", seasonImage["fileName"]);
               image.insert("bannerType", seasonImage["keyType"]);
               image.insert("bannerType2", "");
               image.insert("language", seasonImage["languageId"]);
               image.insert("season", seasonImage["subKey"]);

               return image;
           });

           emit storeSeasonImages(seriesId, seasonImages);
       }

       reply->deleteLater();
    });
}

void Api::getPosterImages(const int &seriesId)
{
    QUrl url(QString("%1/series/%2/images/query?keyType=poster").arg(QString(MIRRORPATH)).arg(seriesId));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull())
       {
           auto posters = parseJson(jsonDocument.object());
           emit storePosterImageFor(seriesId, findHighestRatedImage(posters));
       }

       reply->deleteLater();
    });
}

void Api::getBannerImages(const int &seriesId)
{
    QUrl url(QString("%1/series/%2/images/query?keyType=series").arg(QString(MIRRORPATH)).arg(seriesId));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull())
       {
           auto banners = parseJson(jsonDocument.object());
           emit storeBannerImageFor(seriesId, findHighestRatedImage(banners));
       }

       reply->deleteLater();
    });
}

void Api::getFanartImages(const int &seriesId)
{
    QUrl url(QString("%1/series/%2/images/query?keyType=fanart").arg(QString(MIRRORPATH)).arg(seriesId));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull())
       {
           auto fanartImages = parseJson(jsonDocument.object());
           emit storeFanartImageFor(seriesId, findHighestRatedImage(fanartImages));
       }

       reply->deleteLater();
    });
}

void Api::getActors(const int &seriesId)
{
    QUrl url(QString("%1/series/%2/actors").arg(QString(MIRRORPATH)).arg(seriesId));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull())
       {
           auto actors = parseJson(jsonDocument.object());
           emit storeActors(seriesId, actors);
       }

       reply->deleteLater();
    });
}

void Api::getEpisodes(const int &seriesId, const int &page)
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
               // this is a safe bet to to turn loading indicator off
               // emit episodesFetched();
               return;
           }

           qDebug() << "store episodes for " << seriesId;
           const auto episodes = parseJson(jsonDocument.object());
           emit storeEpisodes(seriesId, episodes);

           qDebug() << "try to get more episodes for " << seriesId;
           getEpisodes(seriesId, page + 1);
       }
    });
}

QString Api::findHighestRatedImage(const QList<QVariantMap> &images)
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

void Api::replyFinishedError(QNetworkReply *reply)
{
    QVariantMap series;
    series.insert("seriesName", "Error, try again later.");

    QList<QVariantMap> allSeries;
    allSeries.append(series);

    emit readyToPopulateSeries(allSeries);

    reply->deleteLater();
}

QList<QVariantMap> Api::parseJson(const QJsonObject &obj)
{
    QList<QVariantMap> results;
    QJsonArray jsonArray;

    if (obj.value("data").isArray())
    {
        jsonArray = obj.value("data").toArray();
    }

    if (obj.value("data").isObject())
    {
        jsonArray.append(obj.value("data"));
    }

    for (auto item : jsonArray)
    {
        QVariantMap result;

        const auto jsonObject = item.toObject();
        const auto keys = jsonObject.keys();
        for (auto key : keys)
        {
            result.insert(key, jsonObject.value(key).toVariant());
        }

        results.append(result);
    }

    return results;
}
