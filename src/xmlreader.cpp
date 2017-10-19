#include "xmlreader.h"

#define APIKEY "88D0BD893851FA78"
#define MIRRORPATH "https://api.thetvdb.com"

XMLReader::XMLReader(QObject *parent) :
    QObject(parent),
    m_nam(0),
    m_fullRecord(false),
    m_update(false)
{
    m_nam = new QNetworkAccessManager(this);

    connect(m_nam,
            SIGNAL(finished(QNetworkReply*)),
            this,
            SLOT(replyFinished(QNetworkReply*)));

    getAuthenticationToken();
    getJwt();
}

XMLReader::~XMLReader()
{
    delete m_nam;
    m_nam = 0;
}

void XMLReader::getJwt()
{
    QUrl url(QString("%1/login").arg(QString(MIRRORPATH)));
    QNetworkRequest request(url);

    request.setRawHeader("Content-Type", "application/json");

    QByteArray jsonString = "{\"apikey\": \"88D0BD893851FA78\"}";

    m_nam->post(request, jsonString);
}

void XMLReader::getAuthenticationToken()
{
    QByteArray body = "{\"apikey\":\"88D0BD893851FA78\"}";
    QUrl url(QString("%1/login").arg(QString(MIRRORPATH)));

    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QString("application/json")));
    m_nam->post(request, body);
}

QString XMLReader::getLocale()
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

void XMLReader::getLanguages()
{
    QString url = QString("%1/api/%2/languages.xml").arg(QString(MIRRORPATH)).arg(QString(APIKEY));
    qDebug() << "Requesting" << url;
    QUrl finalUrl(url);
    get(finalUrl);
}

void XMLReader::searchSeries(QString text)
{
    m_fullRecord = false;
    QString url = QString("%1/search/series?name=%2").arg(QString(MIRRORPATH)).arg(text);
    qDebug() << "Requesting" << url;
    QUrl finalUrl(url);
    get(finalUrl);
}

void XMLReader::getFullSeriesRecord(QString seriesId, QString method)
{
    if (method == "full") {
        setFullRecordFlag(true);
    } else if (method == "update") {
        setUpdateFlag(true);
    }

    QString url = QString("%1/api/%2/series/%3/all/en.zip").arg(QString(MIRRORPATH)).arg(QString(APIKEY)).arg(seriesId);
    qDebug() << "Requesting" << url;
    QUrl finalUrl(url);
    get(finalUrl);
}

void XMLReader::getFullSeriesRecordNew(QString seriesId, QString method)
{
    if (method == "full") {
        setFullRecordFlag(true);
    } else if (method == "update") {
        setUpdateFlag(true);
    }

    QUrl url(QString("%1/series/%2").arg(QString(MIRRORPATH)).arg(seriesId));
    qDebug() << "Requesting" << url;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QString("application/json")));
    request.setRawHeader(QByteArray("Authorization"), QString("Bearer " + m_jwt).toUtf8());
    request.setRawHeader(QByteArray("Accept-Language"), QByteArray("en"));
    m_nam->get(request);
}

QList<QVariantMap> XMLReader::getSeries() { return m_series; }

QList<QVariantMap> XMLReader::getEpisodes() { return m_episodes; }

QList<QVariantMap> XMLReader::getBanners() { return m_banners; }

void XMLReader::get(QUrl url)
{
    QNetworkRequest request(url);

    request.setRawHeader(QByteArray("Authorization"), (QString("Bearer %1").arg(m_jwt)).toLocal8Bit());
    request.setRawHeader(QByteArray("Accept-Language"), QByteArray("en"));

    m_nam->get(request);
}

// ---------------------------------------------------
// slots
// ---------------------------------------------------

void XMLReader::replyFinishedNew(QNetworkReply *reply) {

    auto doc = QJsonDocument::fromJson(reply->readAll());

    QJsonObject obj;

    // check validity of the document
    if (!doc.isNull()) {
        if (doc.isObject()) {
            obj = doc.object();
        } else {
            qDebug() << "Document is not an object" << endl;
        }
    } else {
        qDebug() << "Invalid JSON...\n" << endl;
    }

    qDebug() << "keys" << obj.keys();

    if (obj.contains("token")) {
        QString result = obj.value("token").toString();
        qDebug() << "token" << result;
        m_jwt = result;
    } else {

        if (reply->url().toString().contains("/images")) {
            qDebug() << "images";
        } else if (reply->url().toString().contains("/search/series")) {
            qDebug() << "search";
        } else if (reply->url().path().startsWith("/series")) {
            qDebug() << "series";
        }

        m_series.clear();
        m_series = parseSeriesNew(obj);

        if (m_series.size() != 0 and !getFullRecordFlag() and !getUpdateFlag()) {

            setFullRecordFlag(false);
            setUpdateFlag(false);

            emit readyToPopulateSeries();

        } else if (getFullRecordFlag()) {

            setFullRecordFlag(false);
            setUpdateFlag(false);

            qDebug() << "readyToStoreSeries()";
            emit readyToStoreSeries();

        } else if (getUpdateFlag()) {

            setFullRecordFlag(false);
            setUpdateFlag(false);

            emit readyToUpdateSeries();
            qDebug() << "readyToUpdateSeries()";

        }
    }

    reply->deleteLater();
}
        
void XMLReader::getSeriesFinished(QString seriesId, QString method) {

    if (method == "full") {
        setFullRecordFlag(true);
    } else if (method == "update") {
        setUpdateFlag(true);
    }

    QUrl url(QString("%1/series/%2/images").arg(QString(MIRRORPATH)).arg(seriesId));
    qDebug() << "Requesting" << url;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QString("application/json")));
    request.setRawHeader(QByteArray("Authorization"), QString("Bearer " + m_jwt).toUtf8());
    request.setRawHeader(QByteArray("Accept-Language"), QByteArray("en"));
    m_nam->get(request);
}

void XMLReader::replyFinished(QNetworkReply *reply)
{
    qDebug() << reply->url() << reply->errorString();
    
    // Error -> inform user by appending error message to listview
    if (reply->error() != QNetworkReply::NoError) {

        QVariantMap temp;
        temp.insert("SeriesName", "Error, try again later.");
        m_series.clear();
        m_series.append(temp);
        emit readyToPopulateSeries();

        reply->deleteLater();
        return;
    }

    auto document = QJsonDocument::fromJson(reply->readAll());

    if (document.object().contains("token")) {
        m_jwt = document.object().value("token").toString();
        qDebug() << m_jwt;
    }

    // parse json into the QMaps

    if (document.object().contains("data")) {

        auto jsonArray = document.object().value("data").toArray();

        QList<QVariantMap> allSeries;

        for (auto jsonValue : jsonArray) {

            QVariantMap series;

            auto obj = jsonValue.toObject();
            auto keys = obj.keys();

            for (auto key : keys) {
                qDebug() << key;

                series.insert(key, obj.value(key).toVariant());
            }

            allSeries.append(series);
        }

        if (allSeries.size() != 0) {
            m_series = allSeries;
        }

//        if (languages.size() != 0) {
//            m_languages = languages;
//        }

//        if (episodes.size() != 0){
//            m_episodes = episodes;
//        }

//        if (banners.size() != 0) {
//            m_banners = banners;
//        }

        qDebug() << allSeries;
    }

    if (m_series.size() != 0 and !getFullRecordFlag() and !getUpdateFlag()) {
        setFullRecordFlag(false);
        setUpdateFlag(false);
        emit readyToPopulateSeries();
    } else if (getFullRecordFlag()) {
        setFullRecordFlag(false);
        setUpdateFlag(false);
        emit readyToStoreSeries();
    } else if (getUpdateFlag()) {
        setFullRecordFlag(false);
        setUpdateFlag(false);
        emit readyToUpdateSeries();
    }

    reply->deleteLater();
}

QList<QVariantMap> XMLReader::parseSeriesNew(QJsonObject obj) {
    QList<QVariantMap> allSeries;

    QJsonArray foundSeries;

    if (obj.value("data").isArray()) {
        foundSeries = obj.value("data").toArray();
    } else if (obj.value("data").isObject()) {
        foundSeries.append(obj.value("data"));
    }

    foreach (QJsonValue item, foundSeries) {
        QStringList keys = item.toObject().keys();
        QVariantMap series;
        foreach (QString key, keys) {
            series.insert(key, item.toObject().value(key).toVariant());
        }
        allSeries.append(series);
    }
    return allSeries;
}

QVariantMap XMLReader::parseImages(QJsonObject obj) {

    auto data = obj.value("data").toObject();
    auto keys = data.keys();
    QVariantMap images;
    foreach (QString key, keys) {
        images.insert(key, data.value(key).toVariant());
    }
    return images;
}

bool XMLReader::getUpdateFlag() { return m_update; }

void XMLReader::setUpdateFlag(bool state) { m_update = state; }

bool XMLReader::getFullRecordFlag() { return m_fullRecord; }

void XMLReader::setFullRecordFlag(bool state) { m_fullRecord = state; }
