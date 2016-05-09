#include "xmlreader.h"

#define APIKEY "88D0BD893851FA78"
#define MIRRORPATH "http://thetvdb.com"

XMLReader::XMLReader(QObject *parent) :
    QObject(parent),
    myNetWorkAccessManager(0)
{
    myNetWorkAccessManager = new QNetworkAccessManager(this);

    connect(myNetWorkAccessManager,
            SIGNAL(finished(QNetworkReply*)),
            this,
            SLOT(replyFinished(QNetworkReply*)));

    fullRecord = false;
    update = false; // STUPID, FIX THIS SOMEHOW

//    getLanguages();

}

XMLReader::~XMLReader() {

    delete myNetWorkAccessManager;
    myNetWorkAccessManager = 0;
    qDebug() << "destructing xmlreader";

}

QString XMLReader::getLocale() {

    QString systemLocale = QLocale::system().name();
    QString locale = "en";

    int size = myLanguages.size();
    for(int i = 0; i < size; ++i) {
        QMap<QString,QString> temp = myLanguages.at(i);
        if(temp["abbreviation"] == systemLocale) {
            locale = temp["abbreviation"];
        }
    }
    return locale;
}

void XMLReader::getLanguages() {

    QString url = QString(MIRRORPATH) + "/api/" + QString(APIKEY) + "/languages.xml";
    qDebug() << "Requesting" << url;
    QUrl finalUrl(url);
    startRequest(finalUrl);
}

void XMLReader::searchSeries(QString text) {

    fullRecord = false;
    QString locale = getLocale();
    //QString url = QString(MIRRORPATH) + "/api/GetSeries.php?seriesname=" + text + "&language=" + locale;
    QString url = QString(MIRRORPATH) + "/api/GetSeries.php?seriesname=" + text;
    qDebug() << "Requesting" << url;
    QUrl finalUrl(url);
    startRequest(finalUrl);
}

void XMLReader::getFullSeriesRecord(QString seriesid, QString method) {

    if(method == "full") {
        setFullRecordFlag(true);
    } else if(method == "update") {
        setUpdateFlag(true);
    }

//    QString locale = getLocale();

    // QString url = QString(MIRRORPATH) + "/api/" + QString(APIKEY) + "/series/" + seriesid + "/all/fi.xml";
    QString url = QString(MIRRORPATH) + "/api/" + QString(APIKEY) + "/series/" + seriesid + "/all/en.zip";
    // QString url = QString(MIRRORPATH) + "/api/" + QString(APIKEY) + "/series/" + seriesid + "/all/" + locale + ".zip";
    qDebug() << "Requesting" << url;
    QUrl finalUrl(url);
    startRequest(finalUrl);
}

QList<QMap<QString,QString> > XMLReader::getSeries() { return mySeries; }

QList<QMap<QString,QString> > XMLReader::getEpisodes() { return myEpisodes; }

QList<QMap<QString,QString> > XMLReader::getBanners() { return myBanners; }

void XMLReader::startRequest(QUrl url) {

    QNetworkRequest request(url);
    myNetWorkAccessManager->get(request);

}

// ---------------------------------------------------
// slots
// ---------------------------------------------------

void XMLReader::replyFinished(QNetworkReply *reply) {

    qDebug() << "finished";
    qDebug() << getUpdateFlag();

    QByteArray b = reply->readAll();
    QBuffer* buf = new QBuffer(&b);

    // If server is down, do something.
    if(buf->buffer().contains("522: Connection timed out")) {

        QMap<QString,QString> temp;
        temp.insert("SeriesName","Server timeout, try again later.");
        mySeries.clear();
        mySeries.append(temp);
        emit readyToPopulateSeries();

        reply->deleteLater();
        return;
    }

    if (reply->url().toString().endsWith(".zip")) {
        QZipReader *zip = new QZipReader(buf);
        QXmlStreamReader xml(zip->fileData("en.xml"));
        parseXML(xml);
        QXmlStreamReader xml_banners(zip->fileData("banners.xml"));
        parseXML(xml_banners);

        qDebug() << "here";

        if(mySeries.size() != 0 and !getFullRecordFlag() and !getUpdateFlag()) {
            qDebug() << "emit readyToPopulateSeries";
            emit readyToPopulateSeries();
        } else if(getFullRecordFlag()) {
            qDebug() << "emit readyToStoreSeries";
            emit readyToStoreSeries();
        } else if(getUpdateFlag()) {
            qDebug() << "emit readyToUpdateSeries";
            emit readyToUpdateSeries();
        }

        // lets init the values.
        fullRecord = false;
        update = false;

    } else {

        QXmlStreamReader xml(buf->buffer());
        parseXML(xml);

        if(mySeries.size() != 0 and !getFullRecordFlag() and !getUpdateFlag()) {
            emit readyToPopulateSeries();
        } else if(getFullRecordFlag()) {
            emit readyToStoreSeries();
        } else if(getUpdateFlag()) {
            emit readyToUpdateSeries();
        }

        // lets init the values.
        setFullRecordFlag(false);
        setUpdateFlag(false);
    }

    reply->deleteLater();
}

// These are ugly looking but they seem to be fast
// ---------------------------------------------------

void XMLReader::parseXML(QXmlStreamReader& xml) {

    qDebug() << "parse XML";

    QList<QMap<QString,QString> > series;
    QList<QMap<QString,QString> > languages;
    QList<QMap<QString,QString> > episodes;
    QList<QMap<QString,QString> > banners;

    /* We'll parse the XML until we reach end of it.*/
    while(!xml.atEnd() &&
          !xml.hasError()) {
        /* Read next element.*/
        QXmlStreamReader::TokenType token = xml.readNext();
        /* If token is just StartDocument, we'll go to next.*/
        if(token == QXmlStreamReader::StartDocument) {
            continue;
        }
        /* If token is StartElement, we'll see if we can read it.*/
        if(token == QXmlStreamReader::StartElement) {

            /* If it's named Data, we'll go to the next.*/
            if(xml.name() == "Data") {
                continue;
            }
            /* If it's named Languages, we'll go to the next.*/
            if(xml.name() == "Languages") {
                continue;
            }
            /* If it's named Banners, we'll go to the next.*/
            if(xml.name() == "Banners") {
                continue;
            }

            /* If it's named Series, we'll dig the information from there.*/
            if(xml.name() == "Series") {
                series.append(this->parseSeries(xml));
            }
            /* If it's named Episode, we'll dig the information from there.*/
            if(xml.name() == "Episode") {
                episodes.append(this->parseEpisode(xml));
            }
            /* If it's named Language, we'll dig the information from there.*/
            if(xml.name() == "Language") {
                languages.append(this->parseLanguages(xml));
            }
            if(xml.name() == "Banner") {
                banners.append(this->parseBanner(xml));
            }
        }
    }
    /* Error handling. */
    if(xml.hasError()) {
        qDebug() << xml.errorString();
    }
    /* Removes any device() or data from the reader
         * and resets its internal state to the initial state. */
    xml.clear();

    if(languages.size() != 0) {
        myLanguages = languages;
    }

    if(series.size() != 0) {
        mySeries = series;
    }

    if(episodes.size() != 0){
        myEpisodes = episodes;
    }

    if(banners.size() != 0) {
        myBanners = banners;
    }
}

// Base series record parsing.
QMap<QString, QString> XMLReader::parseSeries(QXmlStreamReader &xml) {

    QMap<QString, QString> series;
    /* Let's check that we're really getting a series. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "Series") {
        return series;
    }
    /* Let's get the attributes for series */
    QXmlStreamAttributes attributes = xml.attributes();
    /* Let's check that series has id attribute. */
    if(attributes.hasAttribute("seriesid")) {
        /* We'll add it to the map. */
        series["seriesid"] = attributes.value("seriesid").toString();
    }
    /* Next element... */
    xml.readNext();
    /*
        * We're going to loop over the things because the order might change.
        * We'll continue the loop until we hit an EndElement named Series.
        */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "Series")) {

        if(xml.tokenType() == QXmlStreamReader::StartElement) {

            if(xml.name() == "seriesid") {
                this->addElementDataToMap(xml, series);
            }

            if(xml.name() == "id") {
                this->addElementDataToMap(xml, series);
            }

            if(xml.name() == "Actors") {
                this->addElementDataToMap(xml, series);
            }

            if(xml.name() == "Airs_DayOfWeek") {
                this->addElementDataToMap(xml, series);
            }

            if(xml.name() == "Airs_Time") {
                this->addElementDataToMap(xml, series);
            }

            if(xml.name() == "ContentRating") {
                this->addElementDataToMap(xml, series);
            }

            if(xml.name() == "Genre") {
                this->addElementDataToMap(xml, series);
            }

            if(xml.name() == "Rating") {
                this->addElementDataToMap(xml, series);
            }

            if(xml.name() == "RatingCount") {
                this->addElementDataToMap(xml, series);
            }

            if(xml.name() == "Runtime") {
                this->addElementDataToMap(xml, series);
            }

            if(xml.name() == "Status") {
                this->addElementDataToMap(xml, series);
            }

            if(xml.name() == "added") {
                this->addElementDataToMap(xml, series);
            }

            if(xml.name() == "addedBy") {
                this->addElementDataToMap(xml, series);
            }

            if(xml.name() == "fanart") {
                this->addElementDataToMap(xml, series);
            }

            if(xml.name() == "lastupdated") {
                this->addElementDataToMap(xml, series);
            }

            if(xml.name() == "poster") {
                this->addElementDataToMap(xml, series);
            }

            /*------------------------------------------*/
            /* These come from searchSeries             */
            /*------------------------------------------*/

            /* We've found Language. */
            if(xml.name() == "Language") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found SeriesName. */
            if(xml.name() == "SeriesName") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found AliasNames. */
            if(xml.name() == "AliasNames") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found banner. */
            if(xml.name() == "banner") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found Overview. */
            if(xml.name() == "Overview") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found FirstAired. */
            if(xml.name() == "FirstAired") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found IMDB_ID. */
            if(xml.name() == "IMDB_ID") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found zap2it_id. */
            if(xml.name() == "zap2it_id") {
                this->addElementDataToMap(xml, series);
            }
            /* We've found Network. */
            if(xml.name() == "Network") {
                this->addElementDataToMap(xml, series);
            }
        }
        /* ...and next... */
        xml.readNext();
    }
    return series;
}

QMap<QString,QString> XMLReader::parseLanguages(QXmlStreamReader &xml) {

    QMap<QString, QString> languages;
    /* Let's check that we're really getting a language. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "Languages") {
        return languages;
    }
    /* Let's get the attributes for language */
    QXmlStreamAttributes attributes = xml.attributes();
    /* Let's check that language has id attribute. */
    if(attributes.hasAttribute("id")) {
        /* We'll add it to the map. */
        languages["id"] = attributes.value("id").toString();
    }
    /* Next element... */
    xml.readNext();
    /*
        * We're going to loop over the things because the order might change.
        * We'll continue the loop until we hit an EndElement named Language.
        */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "Language")) {
        if(xml.tokenType() == QXmlStreamReader::StartElement) {
            /* We've found name. */
            if(xml.name() == "name") {
                this->addElementDataToMap(xml, languages);
            }
            /* We've found abbreviation. */
            if(xml.name() == "abbreviation") {
                this->addElementDataToMap(xml, languages);
            }
        }
        /* ...and next... */
        xml.readNext();
    }
    return languages;
}

// Full episode record parsing.
QMap<QString, QString> XMLReader::parseEpisode(QXmlStreamReader &xml) {

    QMap<QString, QString> episode;
    /* Let's check that we're really getting a series. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "Episode") {
        return episode;
    }
    /* Let's get the attributes for series */
    QXmlStreamAttributes attributes = xml.attributes();
    /* Let's check that series has id attribute. */
    if(attributes.hasAttribute("id")) {
        /* We'll add it to the map. */
        episode["id"] = attributes.value("id").toString();
    }
    /* Next element... */
    xml.readNext();
    /*
        * We're going to loop over the things because the order might change.
        * We'll continue the loop until we hit an EndElement named Series.
        */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "Episode")) {

        if(xml.tokenType() == QXmlStreamReader::StartElement) {

            if(xml.name() == "id") {
                this->addElementDataToMap(xml, episode);
            }

            if(xml.name() == "Director") {
                this->addElementDataToMap(xml, episode);
            }

            if(xml.name() == "EpisodeName") {
                this->addElementDataToMap(xml, episode);
            }

            if(xml.name() == "EpisodeNumber") {
                this->addElementDataToMap(xml, episode);
            }

            if(xml.name() == "GuestStars") {
                this->addElementDataToMap(xml, episode);
            }

            if(xml.name() == "ProductionCode") {
                this->addElementDataToMap(xml, episode);
            }

            if(xml.name() == "Rating") {
                this->addElementDataToMap(xml, episode);
            }

            if(xml.name() == "RatingCount") {
                this->addElementDataToMap(xml, episode);
            }

            if(xml.name() == "SeasonNumber") {
                this->addElementDataToMap(xml, episode);
            }

            if(xml.name() == "Writer") {
                this->addElementDataToMap(xml, episode);
            }

            if(xml.name() == "absolute_number") {
                this->addElementDataToMap(xml, episode);
            }

            if(xml.name() == "airsafter_season") {
                this->addElementDataToMap(xml, episode);
            }

            if(xml.name() == "airsbefore_episode") {
                this->addElementDataToMap(xml, episode);
            }

            /* Location of the episode image */
            if(xml.name() == "filename") {
                this->addElementDataToMap(xml, episode);
            }

            if(xml.name() == "lastupdated") {
                this->addElementDataToMap(xml, episode);
            }

            if(xml.name() == "seasonid") {
                this->addElementDataToMap(xml, episode);
            }

            if(xml.name() == "seriesid") {
                this->addElementDataToMap(xml, episode);
            }

            if(xml.name() == "thumb_added") {
                this->addElementDataToMap(xml, episode);
            }

            if(xml.name() == "thumb_height") {
                this->addElementDataToMap(xml, episode);
            }

            if(xml.name() == "thumb_width") {
                this->addElementDataToMap(xml, episode);
            }

            /* We've found language. */
            if(xml.name() == "Language") {
                this->addElementDataToMap(xml, episode);
            }

            /* We've found banner. */
            if(xml.name() == "banner") {
                this->addElementDataToMap(xml, episode);
            }

            /* We've found Overview. */
            if(xml.name() == "Overview") {
                this->addElementDataToMap(xml, episode);
            }

            /* We've found FirstAired. */
            if(xml.name() == "FirstAired") {
                this->addElementDataToMap(xml, episode);
            }

            /* We've found IMDB_ID. */
            if(xml.name() == "IMDB_ID") {
                this->addElementDataToMap(xml, episode);
            }
        }
        /* ...and next... */
        xml.readNext();
    }
    return episode;
}

QMap<QString, QString> XMLReader::parseBanner(QXmlStreamReader &xml) {

    QMap<QString, QString> banner;
    /* Let's check that we're really getting a series. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "Banner") {
        return banner;
    }
    /* Let's get the attributes for series */
    QXmlStreamAttributes attributes = xml.attributes();
    /* Let's check that banner has id attribute. */
    if(attributes.hasAttribute("id")) {
        /* We'll add it to the map. */
        banner["id"] = attributes.value("id").toString();
    }
    /* Next element... */
    xml.readNext();
    /*
        * We're going to loop over the things because the order might change.
        * We'll continue the loop until we hit an EndElement named Series.
        */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "Banner")) {

        if(xml.tokenType() == QXmlStreamReader::StartElement) {

            if(xml.name() == "id") {
                this->addElementDataToMap(xml, banner);
            }

            if(xml.name() == "BannerPath") {
                this->addElementDataToMap(xml, banner);
            }

            if(xml.name() == "BannerType") {
                this->addElementDataToMap(xml, banner);
            }

            if(xml.name() == "BannerType2") {
                this->addElementDataToMap(xml, banner);
            }

            if(xml.name() == "Language") {
                this->addElementDataToMap(xml, banner);
            }

            if(xml.name() == "Season") {
                this->addElementDataToMap(xml, banner);
            }
        }
        /* ...and next... */
        xml.readNext();
    }
    return banner;
}

void XMLReader::addElementDataToMap(QXmlStreamReader& xml,
                                    QMap<QString, QString>& map) const {
    /* We need a start element, like <foo> */
    if(xml.tokenType() != QXmlStreamReader::StartElement) {
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
    if(xml.tokenType() != QXmlStreamReader::Characters) {
        return;
    }
    /* Now we can add it to the map.*/
    map.insert(elementName, xml.text().toString());
}

bool XMLReader::getUpdateFlag() {
    return update;
}

void XMLReader::setUpdateFlag(bool state) {
    update = state;
}

bool XMLReader::getFullRecordFlag() {
    return fullRecord;
}

void XMLReader::setFullRecordFlag(bool state) {
    fullRecord = state;
}
