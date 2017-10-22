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
    QString url = QString("%1/api/%2/languages.xml").arg(QString(MIRRORPATH)).arg(QString(APIKEY));
    qDebug() << "Requesting" << url;
    QUrl finalUrl(url);
    get(finalUrl);
}

void Api::searchSeries(QString text)
{
    m_fullRecord = false;

    QUrl url(QString("%1/search/series?name=%2").arg(QString(MIRRORPATH)).arg(text));

    QNetworkReply* reply = get(url);

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

void Api::getSeriesFromApi(QString seriesId, QString method)
{
    if (method == "full") {
        setFullRecordFlag(true);
    } else if (method == "update") {
        setUpdateFlag(true);
    }

    QUrl url(QString("%1/series/%2").arg(QString(MIRRORPATH)).arg(seriesId));
    QNetworkReply* reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull()) {
           auto series = parseSeries(jsonDocument.object());

           if (getFullRecordFlag()) {

               setFullRecordFlag(false);
               setUpdateFlag(false);

               emit readyToStoreSeries();

           } else if (getUpdateFlag()) {

               setFullRecordFlag(false);
               setUpdateFlag(false);

               emit readyToUpdateSeries();;
           }
       }

       reply->deleteLater();
    });
}

void Api::getImages(QString seriesId)
{
    QUrl url(QString("%1/series/%2/images").arg(QString(MIRRORPATH)).arg(seriesId));
    QNetworkReply* reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull()) {
           auto images = parseImages(jsonDocument.object());
           qDebug() << images;
       }
    });
}

void Api::getActors(QString seriesId)
{
    QUrl url(QString("%1/series/%2/actors").arg(QString(MIRRORPATH)).arg(seriesId));
    QNetworkReply* reply = get(url);

    connect(reply, &QNetworkReply::finished, [this, reply]()
    {
       auto jsonDocument = QJsonDocument::fromJson(reply->readAll());

       if (!jsonDocument.isNull()) {
           auto actors = parseActors(jsonDocument.object());
           qDebug() << actors;
       }
    });
}

QList<QVariantMap> Api::getSeries() { return m_series; }

QList<QVariantMap> Api::getEpisodes() { return m_episodes; }

QList<QVariantMap> Api::getBanners() { return m_banners; }

QNetworkReply* Api::get(QUrl url)
{
    QNetworkRequest request(url);

    request.setRawHeader(QByteArray("Content-Type"), QByteArray("application/json"));
    request.setRawHeader(QByteArray("Authorization"), (QString("Bearer %1").arg(m_jwt)).toLocal8Bit());
    request.setRawHeader(QByteArray("Accept-Language"), QByteArray("en"));

    qDebug() << "HEIAHEIA" << request.url();

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

        QStringList keys = item.toObject().keys();
        for (auto key : keys) {
            series.insert(key, item.toObject().value(key).toVariant());
        }

        allSeries.append(series);
    }

    return allSeries;
}

QVariantMap Api::parseImages(QJsonObject obj)
{
    QVariantMap images;

    auto data = obj.value("data").toObject();
    auto keys = data.keys();
    for (auto key : keys) {
        images.insert(key, data.value(key).toVariant());
    }

    return images;
}

QVariantMap Api::parseActors(QJsonObject obj)
{
    QVariantMap actors;

    auto data = obj.value("data").toObject();
    auto keys = data.keys();
    for (auto key : keys) {
        actors.insert(key, data.value(key).toVariant());
    }

    return actors;
}

bool Api::getUpdateFlag() { return m_update; }

void Api::setUpdateFlag(bool state) { m_update = state; }

bool Api::getFullRecordFlag() { return m_fullRecord; }

void Api::setFullRecordFlag(bool state) { m_fullRecord = state; }
