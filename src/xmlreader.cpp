#include "xmlreader.h"
#include <QFile>


XMLReader::XMLReader(QObject *parent) :
    QObject(parent),
    myNetWorkAccessManager(0),
    myApiKey("88D0BD893851FA78"),
    myMirrorPath("http://thetvdb.com"),
    tv1("http://feeds.feedburner.com/ampparit-tv-yle-tv1?format=xml"),
    tv2("http://feeds.feedburner.com/ampparit-tv-yle-tv2?format=xml"),
    mtv3("http://feeds.feedburner.com/ampparit-tv-mtv3?format=xml"),
    nelonen("http://feeds.feedburner.com/ampparit-tv-nelonen?format=xml"),
    sub("http://feeds.feedburner.com/ampparit-tv-sub?format=xml"),
    teema("http://feeds.feedburner.com/ampparit-tv-yle-teema?format=xml"),
    fem("http://feeds.feedburner.com/ampparit-tv-yle-fem?format=xml"),
    jim("http://feeds.feedburner.com/ampparit-tv-jim?format=xml"),
    liv("http://feeds.feedburner.com/ampparit-tv-liv?format=xml"),
    ava("http://feeds.feedburner.com/ampparit-tv-ava?format=xml"),
    tvviisi("http://feeds.feedburner.com/ampparit-tv-tv-viisi?format=xml"),
    kutonen("http://feeds.feedburner.com/ampparit-tv-kutonen?format=xml"),
    fox("http://feeds.feedburner.com/ampparit-tv-fox?format=xml"),
    elokuvat("http://feeds.feedburner.com/ampparit-tv-elokuvat-perus?format=xml")
{
    myNetWorkAccessManager = new QNetworkAccessManager(this);

    connect(myNetWorkAccessManager,
            SIGNAL(finished(QNetworkReply*)),
            this,
            SLOT(replyFinished(QNetworkReply*)));

    connect(myNetWorkAccessManager,
            SIGNAL(sslErrors(QNetworkReply*,QList<QSslError>&)),
            this,
            SLOT(sslErrors(QNetworkReply*,QList<QSslError>&)));

    connect(myNetWorkAccessManager,
            SIGNAL(finished(QNetworkReply*)),
            this,
            SLOT(bannerFetchFinished(QNetworkReply*)));

    connect(this,
            SIGNAL(readyToFetchBanner(QString)),
            this,
            SLOT(fetchBanner(QString)));

    fullRecord = false;

}

XMLReader::~XMLReader() {

}

void XMLReader::getLanguages() {

    QString url = myMirrorPath + "/api/" + myApiKey + "/languages.xml";
    qDebug() << "Requesting" << url;
    QUrl finalUrl(url);
    startRequest(finalUrl);
}

void XMLReader::searchSeries(QString text) {

    //mySeries.clear();
    fullRecord = false;
    QString url = myMirrorPath + "/api/GetSeries.php?seriesname=" + text;
    qDebug() << "Requesting" << url;
    QUrl finalUrl(url);
    startRequest(finalUrl);
}

void XMLReader::getFullSeriesRecord(QString seriesid) {

    QString url = myMirrorPath + "/api/" + myApiKey + "/series/" + seriesid + "/all/en.xml";
    qDebug() << "Requesting" << url;
    QUrl finalUrl(url);
    startRequest(finalUrl);
    fullRecord = true;
}

QList<QMap<QString,QString> > XMLReader::getSeries() {

    qDebug("getSeries()");

    return mySeries;
}

QList<QMap<QString,QString> > XMLReader::getEpisodes() { return myEpisodes; }

QList<QMap<QString,QList<QMap<QString,QString> > > > XMLReader::getTVGuide() {

    return myTVGuide;
}

void XMLReader::startRequest(QUrl url) {

    QNetworkRequest request(url);
    myNetWorkAccessManager->get(request);

}

void XMLReader::getServerTime() {

    QString url = "http://thetvdb.com/api/Updates.php?type=none";
    qDebug() << "Requesting" << url;
    QUrl finalUrl(url);
    startRequest(finalUrl);

}

// ---------------------------------------------------
// slots
// ---------------------------------------------------

void XMLReader::replyFinished(QNetworkReply *reply) {


//    QFile file( "series_info.xml" );
//    file.open(QIODevice::WriteOnly);
//    file.write(reply->readAll());

    QString data = QString(reply->readAll());
    qDebug() << data;
    QXmlStreamReader xml(data);
    qDebug() << "Starting to parse xml.";
    parseXML(xml);

    reply->deleteLater();
}

void XMLReader::sslErrors(QNetworkReply *reply, QList<QSslError> &errors) {

    Q_UNUSED(reply);
    Q_UNUSED(errors);

}

// TODO: take image to model.
void XMLReader::bannerFetchFinished(QNetworkReply *reply) {

//    myBanner = new QPixmap();
//    myBanner->loadFromData(reply->readAll());
//    reply->deleteLater();
//    readyToPopulateSeries();
//    qDebug() << "new signal emitted";
}

void XMLReader::fetchBanner(QString banner) {

    QString url = myMirrorPath + "/" + banner;
    qDebug() << "Requesting" << url;
    QUrl finalUrl(url);
    startRequest(finalUrl);
}

// ---------------------------------------------------

void XMLReader::parseXML(QXmlStreamReader& xml) {

    QList<QMap<QString,QString> > series;
    QList<QMap<QString,QString> > languages;
    QList<QMap<QString,QString> > episodes;
    QList<QMap<QString,QString> > channel;

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
            /* If it's named something that the site offers, we'll go to the next.*/
            if(xml.name() == "title") {
                qDebug() << "found title, lets try to get it";
                xml.readNext();
                currentTVchannel = selectChannel(xml.text().toString());
                qDebug() << xml.text();
                continue;
            }
            if(xml.name() == "subtitle") {
                qDebug() << "found subtitle, lets skip it";
                continue;
            }
            if(xml.name() == "updated") {
                qDebug() << "found update info, lets skip it";
                continue;
            }
            if(xml.name() == "generator") {
                qDebug() << "found generator, lets skip it";
                continue;
            }
            if(xml.name() == "link") {
                qDebug() << "found link, lets skip it";
                continue;
            }
            if(xml.name() == "id") {
                qDebug() << "found id, lets skip it";
                continue;
            }
            if(xml.name() == "atom10:link") {
                qDebug() << "found atom10 link, lets skip it";
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
            /* If it's named entry, we'll dig the information from there.*/
            if(xml.name() == "entry") {
                //qDebug() << "entry found, parsing the contents.";
                // We want the channel to fill with different program's info.
                channel.append(this->parseTVChannel(xml));
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

    myLanguages = languages;
    mySeries = series;
    myEpisodes = episodes;

    // We don't want to append this allways.
    if(channel.size() != 0) {
        QMap<QString,QList<QMap<QString,QString> > > temp;
        temp[currentTVchannel] = channel;
        myTVGuide.append(temp);
        readyToPopulateChannels();
    }

    if(mySeries.size() != 0 and !fullRecord) {
        emit readyToPopulateSeries();
    } else if(fullRecord) {
        emit readyToStoreSeries();
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

                xml.readNext();

                if(xml.tokenType() == QXmlStreamReader::Characters) {
                    QString banner = xml.text().toString();
                    emit readyToFetchBanner(banner);
                    //qDebug() << wholeText;

                }
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

QMap<QString, QString> XMLReader::parseTVChannel(QXmlStreamReader &xml) {

    QMap<QString, QString> program;
    /* Let's check that we're really getting a tvchannel-file. */
    if(xml.tokenType() != QXmlStreamReader::StartElement &&
            xml.name() == "entry") {
        return program;
    }

    /* Next element... */
    xml.readNext();
    /*
        * We're going to loop over the things because the order might change.
        * We'll continue the loop until we hit an EndElement named Series.
        */
    while(!(xml.tokenType() == QXmlStreamReader::EndElement &&
            xml.name() == "entry")) {

        if(xml.tokenType() == QXmlStreamReader::StartElement) {

            /* We've found <updated>. */
            if(xml.name() == "updated") {
                this->addElementDataToMap(xml, program);
                //qDebug() << "found updated";
            }
            /* We've found <link>. */
            if(xml.name() == "link") {
                this->addElementDataToMap(xml, program);
                //qDebug() << "found link";
            }
            /* We've found <id>. */
            if(xml.name() == "id") {
                this->addElementDataToMap(xml, program);
                //qDebug() << "found id";
            }
            /* We've found <title type="html">. */
            if(xml.name() == "title") {

                //qDebug() << "found title";

                // We are between <title> </title> fields now.
                xml.readNext();
                /*
                 * This elements needs to contain Characters so we know it's
                 * actually data, if it's not we'll leave.
                 */
                if(xml.tokenType() == QXmlStreamReader::Characters) {

                    QString wholeText = xml.text().toString();
                    //qDebug() << wholeText;
                    //qDebug() << wholeText.size();

                    // We want to parse these attributes to be able to display them.
                    QStringRef weekday(&wholeText,0,2);
                    QStringRef time(&wholeText,3,5);
                    QStringRef programName(&wholeText,9,wholeText.size()-9);

                    //qDebug() << "weekday " << weekday;
                    //qDebug() << "time " << time;
                    //qDebug() << "name of the program " << programName;

                    /* Now we can add it to the map.*/

                    program.insert("weekday", weekday.toString());
                    program.insert("time", time.toString());
                    program.insert("programName", programName.toString());
                    // We want know which channel this is
                    program.insert("channel",currentTVchannel);
                }
            }
        }
        /* ...and next... */
        xml.readNext();
    }

    //    if(program.size() != 0) {

    //        QMap<QString,QString>::iterator itr = program.begin();
    //        while(itr != program.end()) {
    //            qDebug() << itr.key() << itr.value();
    //            ++itr;
    //        }
    //    }

    return program;
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

void XMLReader::updateTVGuide() {

    QUrl url1(tv1);
    startRequest(url1);

    QUrl url2(tv2);
    startRequest(url2);

    QUrl url3(mtv3);
    startRequest(url3);
}

QString XMLReader::selectChannel(QString text) {

    QStringRef wholeText(&text);

    qDebug() << wholeText;

    if(wholeText.contains("Yle TV1")) {
        return "Yle TV1";
    } else if(wholeText.contains("Yle TV2")) {
        return "Yle TV2";
    } else if(wholeText.contains("MTV3")) {
        return "MTV3";
    } else if(wholeText.contains("Nelonen")) {
        return "Nelonen";
    } else if(wholeText.contains("Sub")) {
        return "Sub";
    } else if(wholeText.contains("Yle Teema")) {
        return "Yle Teema";
    } else if(wholeText.contains("Yle Fem")) {
        return "Yle Fem";
    } else if(wholeText.contains("JIM")) {
        return "JIM";
    } else if(wholeText.contains("Liv")) {
        return "Liv";
    } else if(wholeText.contains("AVA")) {
        return "AVA";
    } else if(wholeText.contains("TV Viisi")) {
        return "TV Viisi";
    } else if(wholeText.contains("Kutonen")) {
        return "Kutonen";
    } else if(wholeText.contains("FOX")) {
        return "FOX";
    } else if(wholeText.contains("Elokuvat peruskanavilta")) {
        return "Elokuvat peruskanavilta";
    } else {
        return "EI KANAVATIETOJA";
    }
}
