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

//    connect(m_nam,
//            SIGNAL(finished(QNetworkReply*)),
//            this,
//            SLOT(replyFinished(QNetworkReply*)));

    connect(m_nam,
            SIGNAL(finished(QNetworkReply*)),
            this,
            SLOT(replyFinishedNew(QNetworkReply*)));

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

void XMLReader::getAuthenticationToken() {

    QByteArray body = "{\"apikey\":\"88D0BD893851FA78\"}";
    QUrl url(QString("%1/login").arg(QString(API_BASE_URL)));

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

void XMLReader::getFullSeriesRecord(QString seriesid, QString method)
{
    if (method == "full") {
        setFullRecordFlag(true);
    } else if (method == "update") {
        setUpdateFlag(true);
    }

    QString url = QString("%1/api/%2/series/%3/all/en.zip").arg(QString(MIRRORPATH)).arg(QString(APIKEY)).arg(seriesid);
    qDebug() << "Requesting" << url;
    QUrl finalUrl(url);
    get(finalUrl);
}

QList<QVariantMap> XMLReader::getSeries() { return m_series; }

void XMLReader::getFullSeriesRecordNew(QString seriesid, QString method)
{
    if (method == "full") {
        setFullRecordFlag(true);
    } else if (method == "update") {
        setUpdateFlag(true);
    }

    QUrl url(QString("%1/series/%2").arg(QString(API_BASE_URL)).arg(seriesid));
    qDebug() << "Requesting" << url;
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QVariant(QString("application/json")));
    request.setRawHeader(QByteArray("Authorization"), QString("Bearer " + m_jwt).toUtf8());
    request.setRawHeader(QByteArray("Accept-Language"), QByteArray("en"));
    m_nam->get(request);
}

QList<QMap<QString,QString> > XMLReader::getEpisodes() { return m_episodes; }

QList<QMap<QString,QString> > XMLReader::getBanners() { return m_banners; }

void XMLReader::get(QUrl url)
{
    QNetworkRequest request(url);

    request.setRawHeader("Authorization", (QString("Bearer %1").arg(m_jwt)).toLocal8Bit());

    m_nam->get(request);
}

// ---------------------------------------------------
// slots
// ---------------------------------------------------

void XMLReader::replyFinishedNew(QNetworkReply *reply) {

    QString response = (QString) reply->readAll();
    qDebug() << response;

    QJsonObject obj;
    QJsonDocument doc = QJsonDocument::fromJson(response.toUtf8());

    // check validity of the document
    if (!doc.isNull()) {
        if (doc.isObject()) {
            obj = doc.object();
        } else {
            qDebug() << "Document is not an object" << endl;
        }
    } else {
        qDebug() << "Invalid JSON...\n" << response << endl;
    }

    qDebug() << "keys" << obj.keys();

    if (obj.contains("token")) {
        QString result = obj.value("token").toString();
        qDebug() << "token" << result;
        m_jwt = result;
    } else {

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
//            emit readyToStoreSeries();

        } else if (getUpdateFlag()) {

            setFullRecordFlag(false);
            setUpdateFlag(false);

//            emit readyToUpdateSeries();
            qDebug() << "readyToUpdateSeries()";

        }
    }

    reply->deleteLater();
}

void XMLReader::replyFinished(QNetworkReply *reply)
{
    qDebug() << reply->url() << reply->errorString();
        
    // Error -> inform user by appending error message to listview
    if (reply->error() != QNetworkReply::NoError) {

        QVariantMap temp;
        temp.insert("SeriesName", "Error, try again later.");
        m_series.clear();
//        m_series.append(temp);
//        emit readyToPopulateSeries();

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

void XMLReader::parseXML(QXmlStreamReader& xml)
{
    QList<QMap<QString,QString> > series;
    QList<QMap<QString,QString> > languages;
    QList<QMap<QString,QString> > episodes;
    QList<QMap<QString,QString> > banners;

    /* We'll parse the XML until we reach end of it.*/
    while (!xml.atEnd() &&
          !xml.hasError()) {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xml.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if (token == QXmlStreamReader::StartDocument) {
            continue;
        }
        /* If token is StartElement, we'll see if we can read it.*/
        if (token == QXmlStreamReader::StartElement) {

            /* If it's named Data, we'll go to the next.*/
            if (xml.name() == "Data") {
                continue;
            }
            /* If it's named Languages, we'll go to the next.*/
            if (xml.name() == "Languages") {
                continue;
            }
            /* If it's named Banners, we'll go to the next.*/
            if (xml.name() == "Banners") {
                continue;
            }

            /* If it's named Series, we'll dig the information from there.*/
            if (xml.name() == "Series") {
                series.append(this->parseSeries(xml));
            }
            /* If it's named Episode, we'll dig the information from there.*/
            if (xml.name() == "Episode") {
                episodes.append(this->parseEpisode(xml));
            }
            /* If it's named Language, we'll dig the information from there.*/
            if (xml.name() == "Language") {
                languages.append(this->parseLanguages(xml));
            }
            if (xml.name() == "Banner") {
                banners.append(this->parseBanner(xml));
            }
        }
    }
    /* Error handling. */
    if (xml.hasError()) {
        qDebug() << xml.errorString();
    }
    /* Removes any device() or data from the reader
         * and resets its internal state to the initial state. */
    xml.clear();

    if (languages.size() != 0) {
        m_languages = languages;
    }

    if (series.size() != 0) {
//        m_series = series;
    }

    if (episodes.size() != 0){
        m_episodes = episodes;
    }

    if (banners.size() != 0) {
        m_banners = banners;
    }
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

// Base series record parsing.
QMap<QString, QString> XMLReader::parseSeries(QXmlStreamReader &xml)
{
    QMap<QString, QString> series;
    /* Let's check that we're really getting a series. */
    if (xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "Series") {
        return series;
    }
    /* Let's get the attributes for series */
    QXmlStreamAttributes attributes = xml.attributes();
    /* Let's check that series has id attribute. */
    if (attributes.hasAttribute("seriesid")) {
        /* We'll add it to the map. */
        series["seriesid"] = attributes.value("seriesid").toString();
    }
    /* Next element... */
    xml.readNext();
    /*
        * We're going to loop over the things because the order might change.
        * We'll continue the loop until we hit an EndElement named Series.
        */
    while (!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "Series")) {

        if (xml.tokenType() == QXmlStreamReader::StartElement) {

            if (xml.name() == "seriesid") {
                this->addElementDataToMap(xml, series);
            }

            if (xml.name() == "id") {
                this->addElementDataToMap(xml, series);
            }

            if (xml.name() == "Actors") {
                this->addElementDataToMap(xml, series);
            }

            if (xml.name() == "Airs_DayOfWeek") {
                this->addElementDataToMap(xml, series);
            }

            if (xml.name() == "Airs_Time") {
                this->addElementDataToMap(xml, series);
            }

            if (xml.name() == "ContentRating") {
                this->addElementDataToMap(xml, series);
            }

            if (xml.name() == "Genre") {
                this->addElementDataToMap(xml, series);
            }

            if (xml.name() == "Rating") {
                this->addElementDataToMap(xml, series);
            }

            if (xml.name() == "RatingCount") {
                this->addElementDataToMap(xml, series);
            }

            if (xml.name() == "Runtime") {
                this->addElementDataToMap(xml, series);
            }

            if (xml.name() == "Status") {
                this->addElementDataToMap(xml, series);
            }

            if (xml.name() == "added") {
                this->addElementDataToMap(xml, series);
            }

            if (xml.name() == "addedBy") {
                this->addElementDataToMap(xml, series);
            }

            if (xml.name() == "fanart") {
                this->addElementDataToMap(xml, series);
            }

            if (xml.name() == "lastupdated") {
                this->addElementDataToMap(xml, series);
            }

            if (xml.name() == "poster") {
                this->addElementDataToMap(xml, series);
            }

            /*------------------------------------------*/
            /* These come from searchSeries             */
            /*------------------------------------------*/

            /* We've found Language. */
            if (xml.name() == "Language") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found SeriesName. */
            if (xml.name() == "SeriesName") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found AliasNames. */
            if (xml.name() == "AliasNames") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found banner. */
            if (xml.name() == "banner") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found Overview. */
            if (xml.name() == "Overview") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found FirstAired. */
            if (xml.name() == "FirstAired") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found IMDB_ID. */
            if (xml.name() == "IMDB_ID") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found zap2it_id. */
            if (xml.name() == "zap2it_id") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found Network. */
            if (xml.name() == "Network") {
                this->addElementDataToMap(xml, series);
            }
        }
        /* ...and next... */
        xml.readNext();
    }
    return series;
}

QMap<QString,QString> XMLReader::parseLanguages(QXmlStreamReader &xml)
{
    QMap<QString, QString> languages;
    /* Let's check that we're really getting a language. */
    if (xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "Languages") {
        return languages;
    }
    /* Let's get the attributes for language */
    QXmlStreamAttributes attributes = xml.attributes();
    /* Let's check that language has id attribute. */
    if (attributes.hasAttribute("id")) {
        /* We'll add it to the map. */
        languages["id"] = attributes.value("id").toString();
    }
    /* Next element... */
    xml.readNext();
    /*
        * We're going to loop over the things because the order might change.
        * We'll continue the loop until we hit an EndElement named Language.
        */
    while (!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "Language")) {
        if (xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found name. */
            if (xml.name() == "name") {
                this->addElementDataToMap(xml, languages);
            }
            /* We've found abbreviation. */
            if (xml.name() == "abbreviation") {
                this->addElementDataToMap(xml, languages);
            }
        }
        /* ...and next... */
        xml.readNext();
    }
    return languages;
}

// Full episode record parsing.
QMap<QString, QString> XMLReader::parseEpisode(QXmlStreamReader &xml)
{
    QMap<QString, QString> episode;
    /* Let's check that we're really getting a series. */
    if (xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "Episode") {
        return episode;
    }
    /* Let's get the attributes for series */
    QXmlStreamAttributes attributes = xml.attributes();
    /* Let's check that series has id attribute. */
    if (attributes.hasAttribute("id")) {
        /* We'll add it to the map. */
        episode["id"] = attributes.value("id").toString();
    }
    /* Next element... */
    xml.readNext();
    /*
        * We're going to loop over the things because the order might change.
        * We'll continue the loop until we hit an EndElement named Series.
        */
    while (!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "Episode")) {

        if (xml.tokenType() == QXmlStreamReader::StartElement) {

            if (xml.name() == "id") {
                this->addElementDataToMap(xml, episode);
            }

            if (xml.name() == "Director") {
                this->addElementDataToMap(xml, episode);
            }

            if (xml.name() == "EpisodeName") {
                this->addElementDataToMap(xml, episode);
            }

            if (xml.name() == "EpisodeNumber") {
                this->addElementDataToMap(xml, episode);
            }

            if (xml.name() == "GuestStars") {
                this->addElementDataToMap(xml, episode);
            }

            if (xml.name() == "ProductionCode") {
                this->addElementDataToMap(xml, episode);
            }

            if (xml.name() == "Rating") {
                this->addElementDataToMap(xml, episode);
            }

            if (xml.name() == "RatingCount") {
                this->addElementDataToMap(xml, episode);
            }

            if (xml.name() == "SeasonNumber") {
                this->addElementDataToMap(xml, episode);
            }

            if (xml.name() == "Writer") {
                this->addElementDataToMap(xml, episode);
            }

            if (xml.name() == "absolute_number") {
                this->addElementDataToMap(xml, episode);
            }

            if (xml.name() == "airsafter_season") {
                this->addElementDataToMap(xml, episode);
            }

            if (xml.name() == "airsbefore_episode") {
                this->addElementDataToMap(xml, episode);
            }

            /* Location of the episode image */
            if (xml.name() == "filename") {
                this->addElementDataToMap(xml, episode);
            }

            if (xml.name() == "lastupdated") {
                this->addElementDataToMap(xml, episode);
            }

            if (xml.name() == "seasonid") {
                this->addElementDataToMap(xml, episode);
            }

            if (xml.name() == "seriesid") {
                this->addElementDataToMap(xml, episode);
            }

            if (xml.name() == "thumb_added") {
                this->addElementDataToMap(xml, episode);
            }

            if (xml.name() == "thumb_height") {
                this->addElementDataToMap(xml, episode);
            }

            if (xml.name() == "thumb_width") {
                this->addElementDataToMap(xml, episode);
            }

            /* We've found language. */
            if (xml.name() == "Language") {
                this->addElementDataToMap(xml, episode);
            }

            /* We've found banner. */
            if (xml.name() == "banner") {
                this->addElementDataToMap(xml, episode);
            }

            /* We've found Overview. */
            if (xml.name() == "Overview") {
                this->addElementDataToMap(xml, episode);
            }

            /* We've found FirstAired. */
            if (xml.name() == "FirstAired") {
                this->addElementDataToMap(xml, episode);
            }

            /* We've found IMDB_ID. */
            if (xml.name() == "IMDB_ID") {
                this->addElementDataToMap(xml, episode);
            }
        }
        /* ...and next... */
        xml.readNext();
    }
    return episode;
}

QMap<QString, QString> XMLReader::parseBanner(QXmlStreamReader &xml)
{
    QMap<QString, QString> banner;
    /* Let's check that we're really getting a series. */
    if (xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "Banner") {
        return banner;
    }
    /* Let's get the attributes for series */
    QXmlStreamAttributes attributes = xml.attributes();
    /* Let's check that banner has id attribute. */
    if (attributes.hasAttribute("id")) {
        /* We'll add it to the map. */
        banner["id"] = attributes.value("id").toString();
    }
    /* Next element... */
    xml.readNext();
    /*
        * We're going to loop over the things because the order might change.
        * We'll continue the loop until we hit an EndElement named Series.
        */
    while (!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "Banner")) {

        if (xml.tokenType() == QXmlStreamReader::StartElement) {

            if (xml.name() == "id") {
                this->addElementDataToMap(xml, banner);
            }

            if (xml.name() == "BannerPath") {
                this->addElementDataToMap(xml, banner);
            }

            if (xml.name() == "BannerType") {
                this->addElementDataToMap(xml, banner);
            }

            if (xml.name() == "BannerType2") {
                this->addElementDataToMap(xml, banner);
            }

            if (xml.name() == "Language") {
                this->addElementDataToMap(xml, banner);
            }

            if (xml.name() == "Season") {
                this->addElementDataToMap(xml, banner);
            }
        }
        /* ...and next... */
        xml.readNext();
    }
    return banner;
}

void XMLReader::addElementDataToMap(QXmlStreamReader& xml, QMap<QString, QString>& map) const
{
    /* We need a start element, like <foo> */
    if (xml.tokenType() != QXmlStreamReader::StartElement) {
        return;
    }
    /* Let's read the name... */
    QString elementName = xml.name().toString();
    /* ...go to the next. */
    xml.readNext();
    /*
     * This elements needs to contain Characters so we know it's
     * actually data, if it's not we'll leave.
     */
    if (xml.tokenType() != QXmlStreamReader::Characters) {
        return;
    }
    /* Now we can add it to the map.*/
    map.insert(elementName, xml.text().toString());
}

bool XMLReader::getUpdateFlag() { return m_update; }

void XMLReader::setUpdateFlag(bool state) { m_update = state; }

bool XMLReader::getFullRecordFlag() { return m_fullRecord; }

void XMLReader::setFullRecordFlag(bool state) { m_fullRecord = state; }
