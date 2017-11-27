#include "api.h"

#define APIKEY "88D0BD893851FA78"
#define MIRRORPATH "https://api.thetvdb.com"

Api::Api(QObject *parent) :
    QObject(parent),
    m_nam(new QNetworkAccessManager),
    m_fullRecord(false),
    m_update(false),
    m_jwt(""),
    m_seriesFinished(false),
    m_episodesFinished(false),
    m_actorsFinished(false),
    m_fanartImagesFinished(false),
    m_seasonImagesFinished(false),
    m_seriesImagesFinished(false),
    m_posterImagesFinished(false)
{
    getAuthenticationToken();

    connect(this, SIGNAL(readyToCheckIfReady()), this, SLOT(checkIfReady()));
}

Api::~Api()
{
    delete m_nam;
    m_nam = 0;
}

void Api::getAuthenticationToken()
{
    QUrl url(QString("%1/login").arg(QString(MIRRORPATH)));
    QNetworkRequest request(url);

    request.setRawHeader("Content-Type", "application/json");

    QByteArray jsonString = "{\"apikey\": \"88D0BD893851FA78\"}";

    QNetworkReply* reply = m_nam->post(request, jsonString);

    connect(reply, &QNetworkReply::finished, [this, reply]()
    {
        auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

        if (!jsonDocument.isNull()) {
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
    QUrl url(QString("%1/languages").arg(QString(MIRRORPATH)).arg(QString(APIKEY)));
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
    m_fullRecord = false;

    QUrl url(QString("%1/search/series?name=%2").arg(QString(MIRRORPATH)).arg(text));

    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply]()
    {
        auto document = QJsonDocument::fromJson(reply->readAll());

        if (!document.isNull()) {
            auto series = parseSeries(document.object());
            emit(readyToPopulateSeries(series));
        }

        reply->deleteLater();
    });
}

void Api::getAll(QString seriesId, QString method)
{
    if (method == "full") {
        setFullRecordFlag(true);
    } else if (method == "update") {
        setUpdateFlag(true);
    }

    getSeries(seriesId);
    getEpisodes(seriesId, 1);
    getActors(seriesId);
    getSeasonImages(seriesId);
    getPosterImages(seriesId);
    getSeriesImages(seriesId);
    getFanartImages(seriesId);
}

void Api::getSeries(QString seriesId)
{
    QUrl url(QString("%1/series/%2").arg(QString(MIRRORPATH)).arg(seriesId));
    QNetworkReply* reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull()) {
           m_series = parseSeries(jsonDocument.object());
           m_seriesFinished = true;
           qDebug() << "series " << m_series.size();
           emit readyToCheckIfReady();
       }

       reply->deleteLater();
    });
}

void Api::getSeasonImages(QString seriesId)
{
    QUrl url(QString("%1/series/%2/images/query?keyType=season").arg(QString(MIRRORPATH)).arg(seriesId));
    QNetworkReply* reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull()) {
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

           m_seasonImages = seasonImages;
           m_seasonImagesFinished = true;

           qDebug() << "season images " << m_seasonImages.size();

           emit readyToCheckIfReady();
       }

       reply->deleteLater();
    });
}

void Api::getPosterImages(QString seriesId)
{
    QUrl url(QString("%1/series/%2/images/query?keyType=poster").arg(QString(MIRRORPATH)).arg(seriesId));
    QNetworkReply* reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull()) {
           m_posterImages = parseJSON(jsonDocument.object());
           m_posterImagesFinished = true;
           qDebug() << "posterImages " << m_posterImages.size();
           emit readyToCheckIfReady();
       }

       reply->deleteLater();
    });
}

void Api::getSeriesImages(QString seriesId)
{
    QUrl url(QString("%1/series/%2/images/query?keyType=series").arg(QString(MIRRORPATH)).arg(seriesId));
    QNetworkReply* reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull()) {
           m_seriesImages = parseJSON(jsonDocument.object());
           m_seriesImagesFinished = true;
           qDebug() << "seriesImages " << m_seriesImages.size();
           emit readyToCheckIfReady();
       }

       reply->deleteLater();
    });
}

void Api::getFanartImages(QString seriesId)
{
    QUrl url(QString("%1/series/%2/images/query?keyType=fanart").arg(QString(MIRRORPATH)).arg(seriesId));
    QNetworkReply* reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull()) {
           m_fanartImages = parseJSON(jsonDocument.object());
           m_fanartImagesFinished = true;
           qDebug() << "fanartImages " << m_fanartImages.size();
           emit readyToCheckIfReady();
       }

       reply->deleteLater();
    });
}

void Api::getActors(QString seriesId)
{
    QUrl url(QString("%1/series/%2/actors").arg(QString(MIRRORPATH)).arg(seriesId));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull()) {
           m_actors = parseJSON(jsonDocument.object());
           m_actorsFinished = true;
           qDebug() << "actors " << m_actors.size();
           emit readyToCheckIfReady();
       }

       reply->deleteLater();
    });
}

void Api::getEpisodes(QString seriesId, int page = 1)
{
    QUrl url(QString("%1/series/%2/episodes?page=%3").arg(QString(MIRRORPATH)).arg(seriesId).arg(page));
    auto reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply, seriesId, page]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull()) {
           if (reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt() == 404) {
               qDebug() << "all episodes (" << m_episodes.size() << ") for " << seriesId << "fetched";
               m_episodesFinished = true;
               emit readyToCheckIfReady();
               return;
           }

           m_episodes += parseJSON(jsonDocument.object());

           qDebug() << "get more episodes for " << seriesId;
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

       if (!jsonDocument.isNull()) {

           auto episode = parseEpisode(jsonDocument.object());
           emit readyToPopulateEpisodeDetails(episode);
       }
    });
}

void Api::checkIfReady()
{
    qDebug() << "checkIfReady";

    if (
            m_seriesFinished &&
            m_episodesFinished &&
            m_actorsFinished &&
            m_posterImagesFinished &&
            m_seasonImagesFinished &&
            m_seriesImagesFinished &&
            m_fanartImagesFinished
        )
    {
        qDebug() << "ready";

        /* find the best rated images and use them */
        auto series = m_series.takeFirst();
        series.insert("poster", findHighestRatedImage(m_posterImages));
        series.insert("fanart", findHighestRatedImage(m_fanartImages));
        series.insert("banner", findHighestRatedImage(m_seriesImages));
        m_series.append(series);

        if (getFullRecordFlag()) {

           setFullRecordFlag(false);
           setUpdateFlag(false);

           emit readyToStoreSeries(m_series, m_episodes, m_seasonImages);

        } else if (getUpdateFlag()) {

           setFullRecordFlag(false);
           setUpdateFlag(false);

           emit readyToUpdateSeries(m_series, m_episodes, m_seasonImages);
        }

        m_series.clear();
        m_seriesFinished = false;
        m_episodes.clear();
        m_episodesFinished = false;
        m_seriesImages.clear();
        m_seriesImagesFinished = false;
        m_seasonImages.clear();
        m_seasonImagesFinished = false;
        m_posterImages.clear();
        m_posterImagesFinished = false;
        m_fanartImages.clear();
        m_fanartImagesFinished = false;
        m_actors.clear();
        m_actorsFinished = false;
    }
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

QList<QVariantMap> Api::series() { return m_series; }

QList<QVariantMap> Api::episodes() { return m_episodes; }

QList<QVariantMap> Api::banners() { return m_seasonImages; }

QNetworkReply* Api::get(QUrl url)
{
    QNetworkRequest request(url);

    request.setRawHeader(QByteArray("Content-Type"), QByteArray("application/json"));
    request.setRawHeader(QByteArray("Authorization"), (QString("Bearer %1").arg(m_jwt)).toLocal8Bit());
    request.setRawHeader(QByteArray("Accept-Language"), QByteArray("en"));

    qDebug() << "REQUESTING" << request.url().toString();

    return m_nam->get(request);
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
    for (auto key : keys) {
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
        for (auto key : keys) {
            result.insert(key, jsonObject.value(key).toVariant());
        }

        results.append(result);
    }

    return results;
}

bool Api::getUpdateFlag() { return m_update; }

void Api::setUpdateFlag(bool state) { m_update = state; }

bool Api::getFullRecordFlag() { return m_fullRecord; }

void Api::setFullRecordFlag(bool state) { m_fullRecord = state; }
