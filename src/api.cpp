#include "api.h"

#define APIKEY "88D0BD893851FA78"
#define MIRRORPATH "https://api.thetvdb.com"

Api::Api(QObject *parent) :
    QObject(parent),
    m_nam(new QNetworkAccessManager),
    m_fullRecord(false),
    m_update(false),
    m_jwt("")
{
    getAuthenticationToken();
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
            m_series = parseSeries(document.object());
            emit(readyToPopulateSeries());
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
    getSeasonImages(seriesId);
    getPosterImages(seriesId);
    getActors(seriesId);
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
           qDebug() << "series " << m_series.size();
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
           m_seasonImages = parseImages(jsonDocument.object());
           qDebug() << "season images " << m_seasonImages.size();
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
           m_posters = parseImages(jsonDocument.object());
           qDebug() << "posters " << m_posters.size();
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
           m_actors = parseActors(jsonDocument.object());
           qDebug() << "actors " << m_actors.size();
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
           if (jsonDocument.object().value("data").toArray().isEmpty()) {
               qDebug() << "all episodes for " << seriesId << "fetched";
               return;
           }

           m_episodes += parseEpisodes(jsonDocument.object());

           qDebug() << "get more episodes for " << seriesId;
           getEpisodes(seriesId, page + 1);
       }
    });
}

void Api::checkIfReady(QString seriesId)
{
    if (!m_series.isEmpty() && m_episodesReady && !m_posters.isEmpty() && !m_seasonImages.isEmpty() && !m_actors.isEmpty())
    {
        if (getFullRecordFlag()) {

           setFullRecordFlag(false);
           setUpdateFlag(false);

           emit readyToStoreSeries();

        } else if (getUpdateFlag()) {

           setFullRecordFlag(false);
           setUpdateFlag(false);

           emit readyToUpdateSeries();
        }
    }
}

QList<QVariantMap> Api::series() { return m_series; }

QList<QVariantMap> Api::episodes() { return m_episodes; }

QList<QVariantMap> Api::banners() { return m_banners; }

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

    m_series.clear();
    m_series.append(temp);

    emit readyToPopulateSeries();

    reply->deleteLater();
}

QList<QVariantMap> Api::parseJson(QJsonObject obj)
{

}

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

QList<QVariantMap> Api::parseImages(QJsonObject obj)
{
    QList<QVariantMap> images;

    auto jsonArray = obj.value("data").toArray();

    for (auto item : jsonArray)
    {
        QVariantMap image;

        auto jsonObject = item.toObject();
        auto keys = jsonObject.keys();
        for (auto key : keys) {
            image.insert(key, jsonObject.value(key).toVariant());
        }

        images.append(image);
    }

    return images;
}

QList<QVariantMap> Api::parseActors(QJsonObject obj)
{
    QList<QVariantMap> actors;

    auto jsonArray = obj.value("data").toArray();

    for (auto item: jsonArray)
    {
        QVariantMap actor;

        auto jsonObject = item.toObject();
        auto keys = jsonObject.keys();
        for (auto key : keys) {
            actor.insert(key, jsonObject.value(key).toVariant());
        }

        actors.append(actor);
    }

    return actors;
}

QList<QVariantMap> Api::parseEpisodes(QJsonObject obj)
{
    QList<QVariantMap> episodes;

    auto jsonArray = obj.value("data").toArray();
    for (auto item : jsonArray)
    {
        QVariantMap episode;

        auto jsonObject = item.toObject();
        auto keys = jsonObject.keys();
        for (auto key : keys) {
            episode.insert(key, jsonObject.value(key).toVariant());
        }

        episodes.append(episode);
    }

    return episodes;
}

bool Api::getUpdateFlag() { return m_update; }

void Api::setUpdateFlag(bool state) { m_update = state; }

bool Api::getFullRecordFlag() { return m_fullRecord; }

void Api::setFullRecordFlag(bool state) { m_fullRecord = state; }
