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
            auto series = parseSearchResults(document.object().value("data").toArray());
            emit readyToPopulateSeries(series);
        }

        reply->deleteLater();
    });
}

void Api::getAll(const int &seriesId)
{
    getSeries(seriesId);
    getEpisodes(seriesId);
}

void Api::getSeries(const int &seriesId)
{
    QUrl url(QString("%1/series/%2/extended").arg(QString(MIRRORPATH)).arg(seriesId));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId]()
    {
        auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

        if (!jsonDocument.isNull() && jsonDocument.object().value("data").isObject())
        {
            auto series = parseSeries(jsonDocument.object().value("data").toObject());
            getTranslations(seriesId, "eng", series);

            auto seasons = parseJsonArray(jsonDocument.object().value("data").toObject().value("seasons").toArray());
            auto seasonImages = toSeasonImages(seasons);
            emit storeSeasonImages(seriesId, seasonImages);
        }

        reply->deleteLater();
    });
}

void Api::getTranslations(const int &seriesId, const QString &language, const QVariantMap &series)
{
    QUrl url(QString("%1/series/%2/translations/%3").arg(QString(MIRRORPATH)).arg(seriesId).arg(language));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId, series]()
    {
        auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

        if (!jsonDocument.isNull())
        {
            auto overview = jsonDocument.object().value("data").toObject().value("overview");
            auto seriesCopy = QVariantMap(series);
            seriesCopy.insert("overview", overview);

            emit storeSeries(seriesCopy);
        } else {
            qDebug() << "Overview is missing, store series without it";
            emit storeSeries(series);
        }

        reply->deleteLater();
    });
}

void Api::getEpisodes(const int &seriesId, const int &page)
{
    QUrl url(QString("%1/series/%2/episodes/official/eng?page=%3").arg(QString(MIRRORPATH)).arg(seriesId).arg(page));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId, page]()
    {
        auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

        if (!jsonDocument.isNull())
        {
            if (jsonDocument.object().value("data").toObject().value("episodes").isUndefined())
            {
                qDebug() << "all episodes for " << seriesId << " fetched";
                // this is a safe bet to to turn loading indicator off
                // emit episodesFetched();
                return;
            }

            qDebug() << "store episodes for " << seriesId;
            const auto episodes = parseEpisodes(jsonDocument.object().value("data").toObject().value("episodes").toArray());
            emit storeEpisodes(seriesId, episodes);

            qDebug() << "try to get more episodes for " << seriesId;
            getEpisodes(seriesId, page + 1);
        }

        reply->deleteLater();
    });
}

QString Api::findHighestRatedImage(const QList<QVariantMap> &artworks, ARTWORK_TYPE type)
{
    QVariantMap result;

    for (auto artwork: artworks)
    {
        if (
                result.isEmpty() &&
                artwork.contains("type") &&
                artwork.contains("score") &&
                artwork["type"].toInt() == type
                )
        {
            result = artwork;
        }

        if (
                artwork.contains("type") &&
                artwork.contains("score") &&
                result.contains("type") &&
                result.contains("score") &&
                artwork["type"].toInt() == type &&
                artwork["score"].toDouble() > result["score"].toDouble()
                )
        {
            result = artwork;
        }
    }

    return result["image"].toString();
}

int Api::findSeasonId(const int &seasonNumber, const QList<QVariantMap> &seasons)
{
    int seasonId = -1;

    for (auto season : seasons)
    {
        if (season["number"].toInt() == seasonNumber)
        {
            qDebug() << "season: " << season;
            seasonId = season["id"].toInt();
            break;
        }
    }

    return seasonId;
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

QVariantMap Api::parseRemoteIds(const QJsonArray &arr)
{
    QVariantMap ids;

    for (auto entry : arr)
    {
        if (entry.isObject())
        {
            ids.insert(entry.toObject().value("sourceName").toString(), entry.toObject().value("id").toString());
        }
    }

    return ids;
}

QVariant Api::parseAirsDays(const QJsonObject &obj)
{
    QStringList days;
    const auto keys = obj.keys();

    for (auto key : keys)
    {
        if (obj.value(key).toBool() == true)
        {
            days.append(key);
        }
    }

    return days.join(",");
}

QString Api::parseGenres(const QJsonArray &arr)
{
    QStringList genres;

    for (auto entry : arr)
    {
        if (entry.isObject())
        {
            genres.append(entry.toObject().value("name").toString());
        }
    }

    return genres.join(",");
}

QVariantMap Api::parseSeries(const QJsonObject &obj)
{
    QVariantMap series;

    const auto keys = obj.keys();

    for (auto key : keys)
    {
        if (key == "name")
        {
            series.insert("seriesName", obj[key].toVariant());
            continue;
        }

        if (key == "airsDays" && obj[key].isObject())
        {
            series.insert(key, parseAirsDays(obj[key].toObject()));
            continue;
        }

        if (key == "remoteIds" && obj[key].isArray())
        {
            series.insert(key, parseRemoteIds(obj[key].toArray()));
            continue;
        }

        if (key == "genres" && obj[key].isArray())
        {
            series.insert(key, parseGenres(obj[key].toArray()));
            continue;
        }

        if (key == "status" && obj[key].isObject())
        {
            series.insert(key, obj[key].toObject().value("name").toString());
            continue;
        }

        if (key == "artworks" && obj[key].isArray())
        {
            auto artworks = parseJsonArray(obj[key].toArray());

            auto poster = findHighestRatedImage(artworks, SERIES_POSTER);
            series.insert("poster", poster);

            auto banner = findHighestRatedImage(artworks, SERIES_BANNER);
            series.insert("banner", banner);

            continue;
        }

        if (key == "seasons" && obj[key].isArray())
        {
            continue;
        }

        if (key == "characters" && obj[key].isArray())
        {
            auto characters = parseJsonArray(obj[key].toArray());
            auto personNames = QSet<QString>();

            for (auto character : characters)
            {
                personNames.insert(character["personName"].toString());
            }

            series.insert("actors", personNames.toList().join("|"));

            continue;
        }

        series.insert(key, obj[key].toVariant());
    }

    return series;
}

QList<QVariantMap> Api::parseSearchResults(const QJsonArray &items)
{
    QList<QVariantMap> results;

    for (auto item : items)
    {
        QVariantMap result;
        const auto keys = item.toObject().keys();

        for (auto key : keys)
        {
            if (key == "name")
            {
                result.insert("seriesName", item.toObject().value(key).toVariant());
                continue;
            }

            if (key == "id")
            {
                continue;
            }

            if (key == "tvdb_id")
            {
                result.insert("id", item.toObject().value(key).toVariant());
                continue;
            }

            if (key == "image_url")
            {
                result.insert("banner", item.toObject().value(key).toVariant());
                continue;
            }

            if (key == "remote_ids" && item.toObject().value(key).isArray())
            {
                auto remoteIds = parseRemoteIds(item.toObject().value(key).toArray());
                result.insert("imdbId", remoteIds["IMDB"]);
                continue;
            }

            result.insert(key, item.toObject().value(key).toVariant());
        }

        results.append(result);
    }

    return results;
}

QList<QVariantMap> Api::parseEpisodes(const QJsonArray &items)
{
    QList<QVariantMap> episodes;

    for (auto item : items)
    {
        QVariantMap episode;
        const auto keys = item.toObject().keys();

        for (auto key : keys)
        {
            if (key == "seasons")
            {
                auto seasonNumber = item.toObject().value("seasonNumber").toInt();
                episode["seasonId"] = findSeasonId(seasonNumber, parseJsonArray(item.toObject().value(key).toArray()));
                continue;
            }

            episode.insert(key, item.toObject().value(key).toVariant());
        }

        episodes.append(episode);
    }

    return episodes;
}

QList<QVariantMap> Api::toSeasonImages(const QList<QVariantMap> &seasons)
{
    QList<QVariantMap> seasonImages;

    for (auto season : seasons)
    {
        if (season["image"].toString().isEmpty())
        {
            continue;
        }

        QVariantMap image;
        image.insert("id", season["id"]);
        image.insert("seriesId", season["seriesId"]);
        image.insert("bannerPath", season["image"]);
        image.insert("bannerType", "season");
        image.insert("bannerType2", "season");
        image.insert("language", "");
        image.insert("season", season["number"]);

        seasonImages.append(image);
    }

    return seasonImages;
}

QVariantMap Api::parseJsonObject(const QJsonObject &item)
{
    QVariantMap result;
    const auto keys = item.keys();

    for (auto key : keys)
    {
        result.insert(key, item.value(key).toVariant());
    }

    return result;
}

QList<QVariantMap> Api::parseJsonArray(const QJsonArray &items)
{
    QList<QVariantMap> results;

    for (auto item : items)
    {
        QVariantMap result;
        const auto keys = item.toObject().keys();

        for (auto key : keys)
        {
            result.insert(key, item.toObject().value(key).toVariant());
        }

        results.append(result);
    }

    return results;
}
